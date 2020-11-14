#ifndef WAVEGENERATOR_SOUND_DEVICE_H
#define WAVEGENERATOR_SOUND_DEVICE_H

#include <signal_generator.h>

#include <memory>

#include <SDL_audio.h>

namespace wave_generator::model {

constexpr size_t kSampleSize = 4096;

class SoundDevice {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        int frequency;
        uint16_t samples;
    };

    explicit SoundDevice(Config config);
    ~SoundDevice();

    void Play();
    void Pause();

    auto IsPlaying() const -> bool;
    auto HasGenerator() const -> bool;
    void SetGenerator(SignalGeneratorPtr generator);

  private:
    using Sample = std::array<float, kSampleSize>;

    void FillStream(SignalGeneratorPtr generator, uint8_t* stream, int length);
    static void PlayCallback(void* device_pointer, uint8_t* stream, int length);

    SDL_AudioSpec audio_spec_{};
    Config config_;
    SignalGeneratorPtr generator_{};
    volatile bool is_playing_{};

    Sample sample_{};
};

}

#endif  // WAVEGENERATOR_SOUND_DEVICE_H
