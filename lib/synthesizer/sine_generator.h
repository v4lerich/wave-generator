#ifndef WAVEGENERATOR_SINE_GENERATOR_H
#define WAVEGENERATOR_SINE_GENERATOR_H

#include <memory>

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class SineGenerator : public SignalGenerator {
  public:
    SineGenerator(double base_frequency, double base_amplitude,
                  std::unique_ptr<SignalGenerator> amplitude_signal,
                  std::unique_ptr<SignalGenerator> frequency_signal,
                  std::unique_ptr<SignalGenerator> phase_signal);

    auto SampleAfter(double step) -> double override;
    void Reset(double position) override;

  private:
    std::unique_ptr<SignalGenerator> accumulated_phase_;

    double base_amplitude_;
    double base_frequency_;
    std::unique_ptr<SignalGenerator> amplitude_;
    std::unique_ptr<SignalGenerator> phase_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_SINE_GENERATOR_H
