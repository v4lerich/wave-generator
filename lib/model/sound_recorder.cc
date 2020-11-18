#include "sound_recorder.h"

namespace wave_generator::model {

SoundRecorder::SoundRecorder(uint32_t sample_rate, size_t channels, float seconds)
    : generators_(channels), seconds_{seconds}, sample_rate_{sample_rate}, channels_{channels} {

}

void SoundRecorder::SetGenerator(size_t channel, SignalGeneratorPtr generator) {
    generators_[channel] = std::move(generator);
}

auto SoundRecorder::Record() -> SoundRecording {
    SoundRecording recording = {
        .sample_rate = sample_rate_,
        .channels = std::vector<SoundRecording::SoundChannel>(channels_),
    };

    double step = 1.0 / sample_rate_;
    for (double time = 0; time < seconds_; time += step) {
        for (size_t channel = 0; channel < channels_; channel++) {
            auto& generator = generators_[channel];
            recording.channels[channel].push_back(generator->SampleAfter(step));
        }
    }
    return recording;
}

}
