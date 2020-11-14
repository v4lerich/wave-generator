#ifndef WAVEGENERATOR_SIGNAL_SAMPLES_GENERATOR_H
#define WAVEGENERATOR_SIGNAL_SAMPLES_GENERATOR_H

#include <SDL_audio.h>
#include <signal_generator.h>

#include <atomic>
#include <memory>
#include <mutex>

namespace wave_generator::model {

class SignalSamplesGenerator {
  public:
    using SignalGeneratorPtr = std::shared_ptr<synthesizer::SignalGenerator>;

    struct Config {
        int frequency;
        size_t buffer_size;
    };

    explicit SignalSamplesGenerator(Config config, SignalGeneratorPtr generator = {});

    auto CanGenerate() const -> bool;
    void SetGenerator(SignalGeneratorPtr generator);

    auto GenerateSamples(float* buffer, size_t samples_count) -> bool;
    void Reset();

  private:
    void GenerateSamples(SignalGeneratorPtr generator, float* buffer, size_t samples_count);

    SignalGeneratorPtr generator_;
    Config config_;

    std::mutex generator_mutex_{};
    std::vector<float> samples_buffer_;
};

}

#endif  // WAVEGENERATOR_SIGNAL_SAMPLES_GENERATOR_H