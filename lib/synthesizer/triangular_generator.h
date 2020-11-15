#ifndef WAVEGENERATOR_TRIANGULAR_GENERATOR_H
#define WAVEGENERATOR_TRIANGULAR_GENERATOR_H

#include <memory>

#include "amplitude_frequency_generator.h"

namespace wave_generator::synthesizer {

class TriangularGenerator : public AmplitudeFrequencyGenerator {
  public:
    TriangularGenerator(double base_amplitude, double base_frequency,
                        std::unique_ptr<SignalGenerator> amplitude_signal,
                        std::unique_ptr<SignalGenerator> frequency_signal);

  protected:
    auto Evaluate(double phase) -> double override;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_TRIANGULAR_GENERATOR_H
