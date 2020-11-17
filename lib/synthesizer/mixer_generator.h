#ifndef WAVEGENERATOR_MIXER_GENERATOR_H
#define WAVEGENERATOR_MIXER_GENERATOR_H

#include <memory>

#include "signal_generator.h"

namespace wave_generator::synthesizer {

class MixerGenerator final : public SignalGenerator {
  public:
    using SignalGeneratorPtr = std::unique_ptr<SignalGenerator>;

    struct MixerInputSignal {
        float coefficient;
        SignalGeneratorPtr generator;
    };

    explicit MixerGenerator(std::vector<MixerInputSignal> input_signals);
    auto SampleAfter(double step) -> double override;

    void Reset() override;

  private:
    std::vector<MixerInputSignal> input_signals_;
    float coefficients_sum_;
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_MIXER_GENERATOR_H
