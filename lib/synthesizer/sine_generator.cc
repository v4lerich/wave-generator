#include "sine_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

SineGenerator::SineGenerator(double base_amplitude, double base_frequency,
                             std::unique_ptr<SignalGenerator> amplitude,
                             std::unique_ptr<SignalGenerator> frequency)
    : AmplitudeFrequencyGenerator(base_amplitude, base_frequency,
                                  std::move(amplitude), std::move(frequency)) {}

auto SineGenerator::Evaluate(double phase) -> double {
    return sin(M_PI_2 * phase);
}

}  // namespace wave_generator::synthesizer
