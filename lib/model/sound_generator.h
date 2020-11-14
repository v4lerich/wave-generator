#ifndef WAVEGENERATOR_SOUND_GENERATOR_H
#define WAVEGENERATOR_SOUND_GENERATOR_H

#include <SDL_audio.h>
#include <signal_generator.h>

#include <atomic>
#include <memory>
#include <mutex>

namespace wave_generator::model {


class SoundGenerator {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        int frequency;
        size_t samples;
        size_t buffer_size;
    };

    explicit SoundGenerator(Config config, SignalGeneratorPtr generator = {});

    auto CanGenerate() const -> bool;
    void SetGenerator(SignalGeneratorPtr generator);

    void GenerateSamples(SignalGeneratorPtr generator, uint8_t* buffer, size_t samples_count) const;
    auto GenerateSamples(uint8_t* buffer, size_t samples_count) const -> bool;
    void Reset();

    auto GetFormat() const -> SDL_AudioFormat;

  private:
    SignalGeneratorPtr generator_;
    Config config_;

    mutable std::mutex generator_mutex_{};
    mutable std::vector<float> samples_buffer_;
};

}

#endif  // WAVEGENERATOR_SOUND_GENERATOR_H