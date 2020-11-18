#ifndef WAVEGENERATOR_WAV_RECORDER_VIEW_H
#define WAVEGENERATOR_WAV_RECORDER_VIEW_H

#include "view.h"
#include <cstdint>
#include <future>
#include <sound_recorder.h>

namespace wave_generator::view {

class WavRecorderView : public View {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;
    using CreateGeneratorsFunc = std::function<std::vector<SignalGeneratorPtr>()>;

    WavRecorderView(CreateGeneratorsFunc generators_factory);
    void Render() override;
    void Open();

  private:
    void StartRecording();
    void StartSaving();
    void SaveWavFile(std::string file_path, model::SoundRecording recording);

    bool is_record_config_{};
    bool is_recording_{};
    bool is_saving_{};

    uint32_t sample_rate_;
    float seconds_{};
    std::future<model::SoundRecording> sound_recording_{};
    CreateGeneratorsFunc generators_factory_;
    bool is_saving_error_{};
};

}

#endif  // WAVEGENERATOR_WAV_RECORDER_VIEW_H
