#ifndef WAVEGENERATOR_CACHED_SIGNAL_SAMPLES_GENERATOR_H
#define WAVEGENERATOR_CACHED_SIGNAL_SAMPLES_GENERATOR_H

#include <atomic>
#include <condition_variable>
#include <list>
#include <memory>
#include <thread>

#include "signal_samples_generator.h"

namespace wave_generator::model {

class CachedSignalSamplesGenerator {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        size_t samples_chunk_size;
        size_t cache_samples_chunks;
        SignalSamplesGenerator::Config generator_config;
    };

    explicit CachedSignalSamplesGenerator(Config config);

    void SetGenerator(SignalGeneratorPtr generator);
    void Reset();
    void Start();
    void Stop();
    void Shutdown();

    void GenerateSamples(float* buffer, size_t size);

  private:
    using SamplesChunk = std::vector<float>;
    using SamplesChunkContainer = std::list<SamplesChunk>;

    auto GenerateChunk() -> SamplesChunk;
    void CacheChunk(SamplesChunk chunk);
    auto UnCacheChunks(size_t count) -> SamplesChunkContainer;
    void DoCacheChunks();

    SamplesChunk cache_chunk_{};
    SamplesChunk::iterator cache_chunk_it_{};
    SamplesChunkContainer cache_{};
    std::atomic<size_t> cache_chunk_count_{};
    std::atomic<bool> is_caching_{};

    std::condition_variable is_caching_cond_{};
    std::condition_variable is_cache_not_empty_{};
    std::condition_variable is_cache_not_full_{};
    std::mutex cache_mutex_{};
    std::mutex is_caching_mutex_{};

    std::thread cacher_thread_;
    std::atomic<bool> is_shutdown_{};
    Config config_;
    SignalSamplesGenerator generator_;
};

}

#endif  // WAVEGENERATOR_CACHED_SIGNAL_SAMPLES_GENERATOR_H
