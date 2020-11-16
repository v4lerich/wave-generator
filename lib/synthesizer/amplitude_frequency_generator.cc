#include "amplitude_frequency_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

AmplitudeFrequencyGenerator::AmplitudeFrequencyGenerator(
    double base_amplitude, double base_frequency,
    std::unique_ptr<SignalGenerator> amplitude_signal,
    std::unique_ptr<SignalGenerator> frequency_signal)
    : base_amplitude_{base_amplitude}, base_frequency_{base_frequency} {
    if (frequency_signal) {
        accumulated_position_shift_ =
            std::make_unique<Integrator>(std::move(frequency_signal));
    } else {
        accumulated_position_shift_ = std::make_unique<ConstantGenerator>(0);
    }

    if (amplitude_signal) {
        amplitude_ = std::move(amplitude_signal);
    } else {
        amplitude_ = std::make_unique<ConstantGenerator>(0);
    }
}

auto AmplitudeFrequencyGenerator::SampleAfter(double step) -> double {
    double position = SignalGenerator::SampleAfter(step);
    double accumulated_position_shift = accumulated_position_shift_->SampleAfter(step);

    double phase = base_frequency_ * (accumulated_position_shift + position);

    double amplitude_shift = amplitude_->SampleAfter(step);
    double value = base_amplitude_ * (1 + amplitude_shift) * Evaluate(phase);
    return value;
}

void AmplitudeFrequencyGenerator::Reset() {
    SignalGenerator::Reset();
    if (amplitude_) amplitude_->Reset();
    if (accumulated_position_shift_) accumulated_position_shift_->Reset();
}

}  // namespace wave_generator::synthesizer
