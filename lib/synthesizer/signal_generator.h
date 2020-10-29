#ifndef WAVEGENERATOR_SIGNAL_GENERATOR_H
#define WAVEGENERATOR_SIGNAL_GENERATOR_H

#include <vector>

namespace wave_generator::synthesizer {

class SignalGenerator {
  public:
    virtual auto SampleAfter(double step) -> double;
    virtual void Reset(double position);

  protected:
    void MovePosition(double step);
    void ResetPosition(double position);
    auto Position() const -> double;

  private:
    double position_{0};
};

}  // namespace wave_generator::synthesizer

#endif  // WAVEGENERATOR_SIGNAL_GENERATOR_H
