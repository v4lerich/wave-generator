#include "wav_recorder_view.h"

#include <ImGuiFileDialog.h>
#include <imgui.h>
#include <wav_format.h>

#include <string>
#include <utility>
#include <vector>
#include <fstream>

namespace wave_generator::view {

static const std::string kRecordConfigureWindowName = "Configure recording";
static const std::string kSaveDialogWindowName = "save_dialog";
static const std::vector<uint32_t> kSampleRates = {
    48000,
    44100,
};

WavRecorderView::WavRecorderView(CreateGeneratorsFunc generators_factory)
    : sample_rate_{kSampleRates.front()},
    generators_factory_{std::move(generators_factory)} {
}

void WavRecorderView::Open() {
    if (is_record_config_ || is_recording_ || is_saving_) {
        return;
    }
    is_record_config_ = true;
}

void WavRecorderView::Render() {
    if (is_record_config_) ImGui::OpenPopup(kRecordConfigureWindowName.c_str());

    if (ImGui::BeginPopupModal(kRecordConfigureWindowName.c_str(), &is_record_config_, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::BeginCombo("##sample_rate", ("Sample rate: " + std::to_string(sample_rate_)).c_str())) {
            for (auto& sample_rate : kSampleRates) {
                bool is_selected = sample_rate == sample_rate_;
                if (ImGui::Selectable(std::to_string(sample_rate).c_str(), is_selected)) {
                    sample_rate_ = sample_rate;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndPopup();
        }

        ImGui::InputFloat("Seconds", &seconds_, 0, 0);
        if (ImGui::Button("Start recording")) {
            StartRecording();
        }
        ImGui::EndPopup();
    }

    if (is_recording_) {
        auto window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        if (ImGui::BeginPopupModal("Recording", nullptr, window_flags)) {
            ImGui::Text("Recording in progress...");
            ImGui::EndPopup();
        }
        if (sound_recording_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            StartSaving();
        }
    }

    auto igfd_instance = igfd::ImGuiFileDialog::Instance();

    if (is_saving_) {
        igfd_instance->OpenModal(kSaveDialogWindowName, "Save File", ".wav", ".", "recording");
    }

    if (igfd_instance->FileDialog(kSaveDialogWindowName, ImGuiWindowFlags_NoCollapse, ImVec2{0, 300})) {
        is_saving_ = false;
        if (igfd_instance->IsOk) {
            SaveWavFile(igfd_instance->GetFilePathName(), sound_recording_.get());
        }
        igfd_instance->CloseDialog(kSaveDialogWindowName);
    }
}

void WavRecorderView::StartRecording() {
    is_record_config_ = false;
    is_recording_ = true;
    auto generators = generators_factory_();

    model::SoundRecorder sound_recorder{sample_rate_, generators.size(), float(seconds_)};
    for (size_t channel = 0; channel < generators.size(); channel++) {
        sound_recorder.SetGenerator(channel, std::move(generators[channel]));
    }

    std::packaged_task<model::SoundRecording(model::SoundRecorder)> task{[] (model::SoundRecorder sound_recorder) {
        return sound_recorder.Record();
    }};
    sound_recording_ = task.get_future();

    std::thread task_thread{std::move(task), std::move(sound_recorder)};
    task_thread.detach();
}

void WavRecorderView::StartSaving() {
    is_recording_ = false;
    is_saving_ = true;
}

void WavRecorderView::SaveWavFile(std::string file_path, model::SoundRecording recording) {
    std::ofstream os(file_path, std::ios::binary);
    if (!os) {
        is_saving_error_ = true;
        return;
    }

    model::WavFormatEncoder encoder{{.sampling_rate = recording.sample_rate}};

    std::vector<model::WavSignalChannel<float>> channels;
    for (auto& channel : recording.channels) {
        channels.push_back(std::move(channel));
    }

    encoder.Encode(os, channels);
    os.close();
}

}