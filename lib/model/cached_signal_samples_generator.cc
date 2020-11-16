#include "cached_signal_samples_generator.h"

#include <utility>

namespace wave_generator::model {

CachedSignalSamplesGenerator::CachedSignalSamplesGenerator(
    Config config)
    : config_{config},
    generator_{config.generator_config},
    cacher_thread_{&CachedSignalSamplesGenerator::DoCacheChunks, this} {
    Reset();
}

auto CachedSignalSamplesGenerator::UnCacheChunks(size_t count) -> SamplesChunkContainer {
    SamplesChunkContainer chunk_entries;
    {
        std::unique_lock<std::mutex> lock(cache_mutex_);
        is_cache_not_empty_.wait(lock, [&] () {
            return cache_chunk_count_ >= count || is_shutdown_ || !is_caching_;
        });

        if (is_shutdown_ || !is_caching_) {
            return chunk_entries;
        }

        chunk_entries.splice(
            std::begin(chunk_entries),
            cache_,
            std::begin(cache_));

        cache_chunk_count_ -= count;
        is_cache_not_empty_.notify_all();
        is_cache_not_full_.notify_all();
    }
    return chunk_entries;
}

void CachedSignalSamplesGenerator::CacheChunk(SamplesChunk chunk) {
    SamplesChunkContainer chunk_entry{std::move(chunk)};
    {
        std::unique_lock<std::mutex> lock(cache_mutex_);
        is_cache_not_full_.wait(lock, [&] () {
            return (cache_chunk_count_ < config_.cache_samples_chunks) || is_shutdown_ || !is_caching_;
        });

        if (is_shutdown_ || !is_caching_) {
            return;
        }

        cache_.splice(std::end(cache_), chunk_entry);
        cache_chunk_count_++;

        is_cache_not_full_.notify_all();
        is_cache_not_empty_.notify_all();
    }
}

auto CachedSignalSamplesGenerator::GenerateChunk() -> SamplesChunk {
    SamplesChunk chunk(config_.samples_chunk_size);
    generator_.GenerateSamples(chunk.data(), config_.samples_chunk_size);
    return chunk;
}

void CachedSignalSamplesGenerator::Shutdown() {
    is_shutdown_ = true;
    is_caching_ = false;

    is_caching_cond_.notify_all();
    is_cache_not_full_.notify_all();
    is_cache_not_empty_.notify_all();

    if (cacher_thread_.joinable())
        cacher_thread_.join();
}

void CachedSignalSamplesGenerator::Stop() {
    is_caching_ = false;

    is_caching_cond_.notify_all();
    is_cache_not_full_.notify_all();
    is_cache_not_empty_.notify_all();
}

void CachedSignalSamplesGenerator::Start() {
    is_caching_ = true;

    is_caching_cond_.notify_all();
    is_cache_not_empty_.notify_all();
    is_cache_not_full_.notify_all();
}

void CachedSignalSamplesGenerator::Reset() {
    generator_.Reset();
    cache_.clear();
    cache_chunk_.clear();
    cache_chunk_it_ = std::end(cache_chunk_);
    cache_chunk_count_ = 0;
}

void CachedSignalSamplesGenerator::DoCacheChunks() {
    while (!is_shutdown_) {
        {
            std::unique_lock<std::mutex> lock(is_caching_mutex_);
            is_caching_cond_.wait(lock, [&] () { return is_caching_ || is_shutdown_; });
        }
        if (is_shutdown_) {
            break;
        }

        SamplesChunk chunk = GenerateChunk();
        CacheChunk(chunk);
    }
}

void CachedSignalSamplesGenerator::GenerateSamples(float* buffer, size_t size) {
    while (size > 0) {
        if (cache_chunk_it_ == std::end(cache_chunk_)) {
            auto chunks = UnCacheChunks(1);
            if (chunks.empty()) { break; }

            cache_chunk_ = chunks.front();
            cache_chunk_it_ = std::begin(cache_chunk_);
        }

        auto batch_size = std::min(ssize_t(size), std::distance(cache_chunk_it_, std::end(cache_chunk_)));
        std::copy(cache_chunk_it_, std::next(cache_chunk_it_, batch_size), buffer);

        size -= batch_size;
        std::advance(buffer, batch_size);
        std::advance(cache_chunk_it_, batch_size);
    }
}

CachedSignalSamplesGenerator::~CachedSignalSamplesGenerator() {
    Shutdown();
    if (cacher_thread_.joinable()) {
        cacher_thread_.join();
    }
}

auto CachedSignalSamplesGenerator::GetQueueSize() -> size_t { return cache_chunk_count_; }

void CachedSignalSamplesGenerator::SetGenerator(
    size_t channel, SignalGeneratorPtr generator) {
    generator_.SetGenerator(channel, std::move(generator));
}

void CachedSignalSamplesGenerator::UpdateConfig(Config config) {
    bool is_caching = is_caching_;
    if (is_caching)
        Stop();

    config_ = config;
    generator_.UpdateConfig(config.generator_config);
    Reset();

    if (is_caching)
        Start();
}

}