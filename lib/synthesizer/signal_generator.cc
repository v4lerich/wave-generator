#include "signal_generator.h"

namespace wave_generator::synthesizer {

void SignalGenerator::MovePosition(double step) { position_ += step; }

void SignalGenerator::ResetPosition(double position) { position_ = position; }

auto SignalGenerator::Position() const -> double { return position_; }

auto SignalGenerator::SampleAfter(double step) -> double {
    MovePosition(step);
    return position_;
}

void SignalGenerator::Reset(double position) { ResetPosition(position); }

}  // namespace wave_generator::synthesizer
