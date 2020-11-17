#ifndef WAVEGENERATOR_WHITE_NOISE_GENERATOR_H
#define WAVEGENERATOR_WHITE_NOISE_GENERATOR_H

#include <memory>
#include <random>

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class WhiteNoiseGenerator : public SignalGenerator {
  public:
    WhiteNoiseGenerator(double base_amplitude, std::unique_ptr<SignalGenerator> amplitude_signal);

    auto SampleAfter(double step) -> double override;
    void Reset() override;

  private:
    double base_amplitude_;
    std::unique_ptr<SignalGenerator> amplitude_;

    std::mt19937 generator_;
    std::uniform_real_distribution<> distribution_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_WHITE_NOISE_GENERATOR_H
