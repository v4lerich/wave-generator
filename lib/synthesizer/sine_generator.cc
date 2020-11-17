#include "sine_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

SineGenerator::SineGenerator(double base_amplitude, double base_frequency,
                             std::unique_ptr<SignalGenerator> amplitude_signal,
                             std::unique_ptr<SignalGenerator> frequency_signal)
    : AmplitudeFrequencyGenerator(base_amplitude, base_frequency, std::move(amplitude_signal),
                                  std::move(frequency_signal)) {}

auto SineGenerator::Evaluate(double phase) -> double { return sin(2 * M_PI * phase); }

}  // namespace wave_generator::synthesizer
