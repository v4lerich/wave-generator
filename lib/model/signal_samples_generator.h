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

    void SetGenerator(size_t channel, SignalGeneratorPtr generator);

    void GenerateSamples(float* buffer, size_t samples_count);
    void Reset();

    void UpdateConfig(Config config);

  private:
    void GenerateSample(float* buffer, double sample_step) const;

    std::vector<SignalGeneratorPtr> generators_;
    Config config_;

    std::mutex mutex_{};
};

}  // namespace wave_generator::model

#endif  // WAVEGENERATOR_SIGNAL_SAMPLES_GENERATOR_H