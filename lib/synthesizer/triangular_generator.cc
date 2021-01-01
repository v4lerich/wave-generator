#include "triangular_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

TriangularGenerator::TriangularGenerator(double base_amplitude, double base_frequency,
                                         std::unique_ptr<SignalGenerator> amplitude,
                                         std::unique_ptr<SignalGenerator> frequency)
    : AmplitudeFrequencyGenerator(base_amplitude, base_frequency, std::move(amplitude),
                                  std::move(frequency)) {}

auto TriangularGenerator::Evaluate(double phase) -> double {
    return M_2_PI * asin(sin(2 * M_PI * phase));
}

}  // namespace wave_generator::synthesizer
