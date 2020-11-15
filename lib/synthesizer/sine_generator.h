#ifndef WAVEGENERATOR_SINE_GENERATOR_H
#define WAVEGENERATOR_SINE_GENERATOR_H

#include <memory>

#include "amplitude_frequency_generator.h"

namespace wave_generator::synthesizer {

class SineGenerator : public AmplitudeFrequencyGenerator {
  public:
    SineGenerator(double base_amplitude, double base_frequency,
                  std::unique_ptr<SignalGenerator> amplitude_signal,
                  std::unique_ptr<SignalGenerator> frequency_signal);

  protected:
    auto Evaluate(double phase) -> double override;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_SINE_GENERATOR_H
