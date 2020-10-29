#ifndef WAVEGENERATOR_TRIANGULAR_GENERATOR_H
#define WAVEGENERATOR_TRIANGULAR_GENERATOR_H

#include <memory>

#include "amplitude_frequency_generator.h"

namespace wave_generator::synthesizer {

class TriangularGenerator : public AmplitudeFrequencyGenerator {
  public:
    TriangularGenerator(double base_frequency, double base_amplitude,
                        std::unique_ptr<SignalGenerator> amplitude_signal,
                        std::unique_ptr<SignalGenerator> frequency_signal);

  protected:
    auto Evaluate(double phase) -> double override;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_TRIANGULAR_GENERATOR_H
