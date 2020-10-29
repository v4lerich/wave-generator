#include "white_noise_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

WhiteNoiseGenerator::WhiteNoiseGenerator(
    double base_amplitude,
    std::unique_ptr<SignalGenerator> amplitude_signal)
    : base_amplitude_{base_amplitude} {

    std::random_device random_device;
    generator_ = std::mt19937{random_device()};
    distribution_ = std::uniform_real_distribution{-1.0, 1.0};

    if (amplitude_signal) {
        amplitude_ = std::move(amplitude_signal);
    } else {
        amplitude_ = std::make_unique<ConstantGenerator>(0);
    }
}

auto WhiteNoiseGenerator::SampleAfter(double step) -> double {
    SignalGenerator::SampleAfter(step);
    double amplitude_shift = amplitude_->SampleAfter(step);
    double value = base_amplitude_ * (1 + amplitude_shift) * distribution_(generator_);
    return value;
}

void WhiteNoiseGenerator::Reset() {
    SignalGenerator::Reset();
    if (amplitude_) amplitude_->Reset();
}

}  // namespace wave_generator::synthesizer
