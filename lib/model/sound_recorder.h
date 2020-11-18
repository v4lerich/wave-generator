#ifndef WAVEGENERATOR_SOUND_RECORDER_H
#define WAVEGENERATOR_SOUND_RECORDER_H

#include <signal_generator.h>

#include <cstdint>
#include <vector>
#include <memory>

namespace wave_generator::model {

struct SoundRecording {
    using SoundChannel = std::vector<float>;

    uint32_t sample_rate;
    std::vector<SoundChannel> channels;
};

class SoundRecorder {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;

    SoundRecorder(uint32_t sample_rate, size_t channels, float seconds);
    void SetGenerator(size_t channel, SignalGeneratorPtr generator);
    auto Record() const -> SoundRecording;

  private:
    std::vector<SignalGeneratorPtr> generators_;
    float seconds_;
    uint32_t sample_rate_;
    size_t channels_;
};

}

#endif  // WAVEGENERATOR_SOUND_RECORDER_H
