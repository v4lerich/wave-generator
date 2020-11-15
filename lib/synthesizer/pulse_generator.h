#ifndef WAVEGENERATOR_PULSE_GENERATOR_H
#define WAVEGENERATOR_PULSE_GENERATOR_H

#include <memory>

#include "amplitude_frequency_generator.h"

namespace wave_generator::synthesizer {

class PulseGenerator : public AmplitudeFrequencyGenerator {
  public:
    PulseGenerator(
        double duty_cycle,
        double base_amplitude, double base_frequency,
        std::unique_ptr<SignalGenerator> amplitude_signal,
        std::unique_ptr<SignalGenerator> frequency_signal);

  protected:
    double Evaluate(double phase) override;

  private:
    double duty_cycle_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_PULSE_GENERATOR_H
