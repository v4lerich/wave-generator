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
        size_t frequency;
        size_t channels;
    };

    explicit SignalSamplesGenerator(Config config);

    auto CanGenerate() const -> bool;
    void SetGenerator(size_t channel, SignalGeneratorPtr generator);

    auto GenerateSamples(float* buffer, size_t samples_count) -> bool;
    void Reset();

    void UpdateConfig(Config config);

  private:
    auto GenerateSample(float* buffer,
                        double sample_step) const -> bool;

    std::vector<SignalGeneratorPtr> generators_;
    Config config_;

    std::mutex mutex_{};
    std::vector<float> samples_buffer_;
};

}

#endif  // WAVEGENERATOR_SIGNAL_SAMPLES_GENERATOR_H