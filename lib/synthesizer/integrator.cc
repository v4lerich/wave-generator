#include "integrator.h"

namespace wave_generator::synthesizer {

Integrator::Integrator(std::unique_ptr<SignalGenerator> signal)
    : signal_{std::move(signal)}, integration_value_{0} {}

void Integrator::Reset() {
    SignalGenerator::Reset();
    integration_value_ = 0;
}

auto Integrator::SampleAfter(double step) -> double {
    SignalGenerator::MovePosition(step);
    integration_value_ += signal_->SampleAfter(step) * step;
    return integration_value_;
}

}  // namespace wave_generator::synthesizer
