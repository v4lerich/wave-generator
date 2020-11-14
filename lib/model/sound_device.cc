#include "sound_device.h"

#include <algorithm>
#include <atomic>
#include <utility>

namespace wave_generator::model {

SoundDevice::SoundDevice(Config config)
    : config_{config}, generator_{SoundGenerator::Config{
        .frequency = config_.frequency,
        .samples = config_.samples,
        .buffer_size = 4096,
    }}, cacher_thread_{&SoundDevice::CacheSamples, this} {
    audio_spec_.freq = config.frequency;
    audio_spec_.callback = GenerateSamples;
    audio_spec_.channels = 1;
    audio_spec_.userdata = this;
    audio_spec_.samples = config.samples;
    audio_spec_.format = AUDIO_F32;

    if (SDL_OpenAudio(&audio_spec_, nullptr) < 0) {
        exit(-1);
    }

    Reset();
}

SoundDevice::~SoundDevice() {
    SDL_CloseAudio();
}

void SoundDevice::SetGenerator(SignalGeneratorPtr generator) {
    generator_.SetGenerator(std::move(generator));
}

auto SoundDevice::IsPlaying() const -> bool { return is_playing_; }

void SoundDevice::Play() {
    generator_.Reset();
    is_playing_ = true;

    is_cache_not_full_.notify_one();

    SDL_PauseAudio(SDL_FALSE);
}

void SoundDevice::Pause() {
    is_playing_ = false;
    SDL_PauseAudio(SDL_TRUE);
}

void SoundDevice::GenerateSamples(void* device_pointer, uint8_t* stream, int length) {
    auto device = static_cast<SoundDevice *>(device_pointer);
    device->GenerateSamples(stream, length);
}

void SoundDevice::GenerateSamples(uint8_t* stream, ssize_t length) {
    while (length > 0) {
        if (cache_chunk_it_ == std::end(cache_chunk_)) {
            auto cached_entries = UnCacheSamplesChunks(1);
            if (cached_entries.empty()) { break; }
            cache_chunk_ = cached_entries.front();
            cache_chunk_it_ = std::begin(cache_chunk_);
        }

        auto batch_length = std::min(length, std::distance(std::end(cache_chunk_), cache_chunk_it_));
        auto old_cache_chunk_it = cache_chunk_it_;
        std::advance(cache_chunk_it_, batch_length);
        std::copy(old_cache_chunk_it, cache_chunk_it_, stream);

        length -= batch_length;
    }
}

void SoundDevice::CacheSamples() {
    while (!thread_should_shutdown_) {
        SamplesChunk chunk(config_.samples_chunk_size * sizeof(float));
        generator_.GenerateSamples(chunk.data(), config_.samples_chunk_size);
        SamplesChunkContainer cache_entry{chunk};

        CacheSamplesChunks(cached_samples_);
    }
}

void SoundDevice::CacheSamplesChunks(SamplesChunkContainer cache_entries) {
    if (!IsCaching())
        return;

    auto size = cache_entries.size();
    std::unique_lock<std::mutex> lock(cache_mutex_);

    is_cache_not_full_.wait(lock, [&]() {
      return (cached_chunks_count_ + size <= config_.cache_samples_chunks) ||
             thread_should_shutdown_;
    });

    if (!thread_should_shutdown_) {
        cached_samples_.splice(std::end(cached_samples_), std::move(cache_entries));
        cached_chunks_count_ += size;
        is_cache_not_empty_.notify_one();
    }
}

auto SoundDevice::UnCacheSamplesChunks(size_t count) -> SamplesChunkContainer {
    SamplesChunkContainer result;

    std::unique_lock<std::mutex> lock{cache_mutex_};

    is_cache_not_empty_.wait(lock, [&] () {
        return (IsCaching() && cached_chunks_count_ >= count) || thread_should_shutdown_;
    });

    if (thread_should_shutdown_) {
        return {};
    }

    result.splice(std::begin(result), cached_samples_, std::next(std::begin(cached_samples_), count - 1));
    cached_chunks_count_ -= count;
    is_cache_not_full_.notify_one();

    return result;
}

void SoundDevice::Reset() {
    std::unique_lock<std::mutex> lock(cache_mutex_);

    generator_.Reset();
    is_playing_ = false;
    cache_chunk_.clear();
    cache_chunk_it_ = std::end(cache_chunk_);
    cached_samples_.clear();
    cached_chunks_count_ = 0;
}

auto SoundDevice::IsCaching() const -> bool {
    return generator_.CanGenerate() && is_playing_;
}

}