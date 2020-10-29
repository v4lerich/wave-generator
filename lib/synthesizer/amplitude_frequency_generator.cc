#include "amplitude_frequency_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

AmplitudeFrequencyGenerator::AmplitudeFrequencyGenerator(
    double base_frequency, double base_amplitude,
    std::unique_ptr<SignalGenerator> amplitude_signal,
    std::unique_ptr<SignalGenerator> frequency_signal)
    : base_amplitude_{base_amplitude}, base_frequency_{base_frequency} {
    if (frequency_signal) {
        accumulated_phase_ =
            std::make_unique<Integrator>(std::move(frequency_signal));
    } else {
        accumulated_phase_ = std::make_unique<ConstantGenerator>(0);
    }

    if (amplitude_signal) {
        amplitude_ = std::move(amplitude_signal);
    } else {
        amplitude_ = std::make_unique<ConstantGenerator>(0);
    }
}

auto AmplitudeFrequencyGenerator::SampleAfter(double step) -> double {
    double position = SignalGenerator::SampleAfter(step);
    double accumulated_phase = accumulated_phase_->SampleAfter(step);

    double phase = accumulated_phase + base_frequency_ * position;

    double amplitude_shift = amplitude_->SampleAfter(step);
    double value = base_amplitude_ * (1 + amplitude_shift) * Evaluate(phase);
    return value;
}

void AmplitudeFrequencyGenerator::Reset() {
    SignalGenerator::Reset();
    if (amplitude_) amplitude_->Reset();
    if (accumulated_phase_) accumulated_phase_->Reset();
}

}  // namespace wave_generator::synthesizer