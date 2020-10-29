#ifndef WAVEGENERATOR_CONSTANT_GENERATOR_H
#define WAVEGENERATOR_CONSTANT_GENERATOR_H

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class ConstantGenerator : public SignalGenerator {
  public:
    explicit ConstantGenerator(double value);

    auto SampleAfter(double step) -> double override;
    void Reset() override;

  private:
    double value_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_CONSTANT_GENERATOR_H
