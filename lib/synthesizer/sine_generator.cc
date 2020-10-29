#include "sine_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

SineGenerator::SineGenerator(double base_amplitude, double base_frequency,
                             std::unique_ptr<SignalGenerator> amplitude,
                             std::unique_ptr<SignalGenerator> frequency,
                             std::unique_ptr<SignalGenerator> phase)
    : base_amplitude_{base_amplitude},
      base_frequency_{base_frequency},
      phase_{std::move(phase)} {
    if (frequency) {
        accumulated_phase_ = std::make_unique<Integrator>(std::move(frequency));
    } else {
        accumulated_phase_ = std::make_unique<ConstantGenerator>(0);
    }

    if (amplitude) {
        amplitude_ = std::move(amplitude);
    } else {
        accumulated_phase_ = std::make_unique<ConstantGenerator>(0);
    }
}

auto SineGenerator::SampleAfter(double step) -> double {
    double position = SignalGenerator::SampleAfter(step);
    double accumulated_phase = accumulated_phase_->SampleAfter(step);
    double phase_value = phase_->SampleAfter(step);

    double phase =
        2 * M_PI *
        (accumulated_phase + phase_value + base_frequency_ * position);

    double amplitude_shift = amplitude_->SampleAfter(step);
    double value = base_amplitude_ * (1 + amplitude_shift) * sin(phase);
    return value;
}

void SineGenerator::Reset(double position) {
    SignalGenerator::Reset(position);
    if (amplitude_) amplitude_->Reset(position);
    if (phase_) phase_->Reset(position);
    if (accumulated_phase_) accumulated_phase_->Reset(position);
}

}  // namespace wave_generator::synthesizer
