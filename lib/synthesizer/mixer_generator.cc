#include "mixer_generator.h"

#include <algorithm>
#include <numeric>
#include <utility>

namespace wave_generator::synthesizer {

MixerGenerator::MixerGenerator(std::vector<MixerInputSignal> input_signals)
    : input_signals_{std::move(input_signals)} {
    coefficients_sum_ = std::accumulate(std::begin(input_signals_), std::end(input_signals_), 0.0F, [] (const auto& accumulator, const auto& mixer_input) {
            return accumulator + mixer_input.coefficient;
        });
}

auto MixerGenerator::SampleAfter(double step) -> double {
    double value = 0;
    for (auto& input_signal : input_signals_) {
        if (auto& generator = input_signal.generator; generator) {
            auto generator_value = generator->SampleAfter(step);
            value += input_signal.coefficient * generator_value;
        }
    }
    value /= coefficients_sum_;
    return value;
}

void MixerGenerator::Reset() {
    for (auto& input_signal : input_signals_) {
        input_signal.generator->Reset();
    }
}

}