#ifndef WAVEGENERATOR_SOUND_DEVICE_H
#define WAVEGENERATOR_SOUND_DEVICE_H

#include <SDL_audio.h>
#include <signal_generator.h>

#include <atomic>
#include <list>
#include <memory>
#include <condition_variable>
#include <thread>

#include "sound_generator.h"

namespace wave_generator::model {

constexpr size_t kSampleSize = 4096;

class SoundDevice {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        int frequency;
        size_t samples;
        size_t samples_chunk_size;
        size_t cache_samples_chunks;
    };

    explicit SoundDevice(Config config);
    ~SoundDevice();

    void Play();
    void Pause();

    auto IsPlaying() const -> bool;
    auto IsCaching() const -> bool;
    void SetGenerator(SignalGeneratorPtr generator);

  private:
    using SamplesChunk = std::vector<uint8_t>;
    using SamplesChunkContainer = std::list<SamplesChunk>;

    void CacheSamplesChunks(SamplesChunkContainer cache_entries);
    void CacheSamples();

    auto UnCacheSamplesChunks(size_t count) -> SamplesChunkContainer;

    void Reset();
    void GenerateSamples(uint8_t* stream, ssize_t length);
    static void GenerateSamples(void* device_pointer, uint8_t* stream, int length);

    Config config_;
    SDL_AudioSpec audio_spec_{};
    SoundGenerator generator_;
    volatile bool is_playing_{};

    SamplesChunk cache_chunk_{};
    SamplesChunk::iterator cache_chunk_it_{};
    SamplesChunkContainer cached_samples_{};
    std::thread cacher_thread_;
    bool thread_should_shutdown_{};

    std::mutex cache_mutex_{};
    std::condition_variable is_cache_not_empty_{};
    std::condition_variable is_cache_not_full_{};
    size_t cached_chunks_count_{};

};

}

#endif  // WAVEGENERATOR_SOUND_DEVICE_H
