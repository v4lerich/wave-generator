#ifndef WAVEGENERATOR_SOUND_DEVICE_H
#define WAVEGENERATOR_SOUND_DEVICE_H

#include <SDL_audio.h>
#include <signal_generator.h>

#include "signal_samples_generator.h"

namespace wave_generator::model {

class SoundDevice {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        SignalSamplesGenerator::Config generator_config;
        size_t samples;
    };

    explicit SoundDevice(Config config);
    ~SoundDevice();

    void Play();
    void Pause();

    auto IsPlaying() const -> bool;
    void SetGenerator(size_t channel, SignalGeneratorPtr generator);

    auto GetConfig() const -> const Config&;
    void UpdateConfig(Config config);

  private:
    void Reset();
    void GenerateSamples(uint8_t* buffer, size_t size);
    static void GenerateSamples(void* device_pointer, uint8_t* buffer, int size);

    Config config_;
    SDL_AudioSpec audio_spec_{};
    SignalSamplesGenerator generator_;
    bool is_playing_{};
};

using SoundDevicePtr = std::shared_ptr<SoundDevice>;

}

#endif  // WAVEGENERATOR_SOUND_DEVICE_H
