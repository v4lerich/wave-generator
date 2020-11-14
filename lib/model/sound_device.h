#ifndef WAVEGENERATOR_SOUND_DEVICE_H
#define WAVEGENERATOR_SOUND_DEVICE_H

#include <SDL_audio.h>
#include <signal_generator.h>

#include "cached_signal_samples_generator.h"
#include "signal_samples_generator.h"

namespace wave_generator::model {

constexpr size_t kSampleSize = 4096;

class SoundDevice {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        int frequency;
        size_t buffer_size;
        size_t samples;
        size_t samples_chunk_size;
        size_t cache_samples_chunks;
    };

    explicit SoundDevice(Config config);
    ~SoundDevice();

    void Play();
    void Pause();

    auto IsPlaying() const -> bool;
    void SetGenerator(SignalGeneratorPtr generator);

  private:
    void Reset();
    void GenerateSamples(uint8_t* buffer, size_t size);
    static void GenerateSamples(void* device_pointer, uint8_t* buffer, int size);

    Config config_;
    SDL_AudioSpec audio_spec_{};
    CachedSignalSamplesGenerator cacher_;
    std::vector<float> buffer_{};
    bool is_playing_{};
};

}

#endif  // WAVEGENERATOR_SOUND_DEVICE_H
