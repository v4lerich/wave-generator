#ifndef WAVEGENERATOR_INTEGRATOR_H
#define WAVEGENERATOR_INTEGRATOR_H

#include <memory>

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class Integrator : public SignalGenerator {
  public:
    explicit Integrator(std::unique_ptr<SignalGenerator> signal);

    auto SampleAfter(double step) -> double override;
    void Reset() override;

  private:
    double integration_value_;

    std::unique_ptr<SignalGenerator> signal_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_INTEGRATOR_H
