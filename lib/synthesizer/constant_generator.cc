#include "constant_generator.h"

namespace wave_generator::synthesizer {

ConstantGenerator::ConstantGenerator(double value) : value_{value} {}

auto ConstantGenerator::SampleAfter(double /*step*/) -> double {
    return value_;
}

void ConstantGenerator::Reset() {}

}  // namespace wave_generator::synthesizer
