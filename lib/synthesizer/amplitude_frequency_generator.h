#ifndef WAVEGENERATOR_AMPLITUDE_PHASE_GENERATOR_H
#define WAVEGENERATOR_AMPLITUDE_PHASE_GENERATOR_H

#include <memory>

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class AmplitudeFrequencyGenerator : public SignalGenerator {
  public:
    AmplitudeFrequencyGenerator(
        double base_amplitude, double base_frequency,
        std::unique_ptr<SignalGenerator> amplitude_signal,
        std::unique_ptr<SignalGenerator> frequency_signal);

    auto SampleAfter(double step) -> double override;
    void Reset() override;

  protected:
    virtual auto Evaluate(double phase) -> double = 0;

  private:
    std::unique_ptr<SignalGenerator> accumulated_position_shift_;

    double base_amplitude_;
    double base_frequency_;
    std::unique_ptr<SignalGenerator> amplitude_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_AMPLITUDE_PHASE_GENERATOR_H
