#include "player_view.h"

#include <IconsForkAwesome.h>
#include <imgui.h>
#include <pulse_generator.h>
#include <sine_generator.h>

namespace wave_generator::view {

static const std::string kWindowName = "player";
static constexpr float kWindowHeight = 20.0;
static constexpr float kWindowPadding = 5.0;
static const std::string kPlayButtonText = std::string(ICON_FK_PLAY) + " Play";
static const std::string kPauseButtonText = std::string(ICON_FK_PAUSE) + " Pause";
static const model::SoundDevice::Config kSoundDeviceConfig = {
    .frequency = 48000,
    .buffer_size = 1 << 14,
    .samples = 1 << 12,
    .samples_chunk_size = 1 << 14,
    .cache_samples_chunks = 100,
};

PlayerView::PlayerView()
    : sound_device_{std::make_shared<model::SoundDevice>(kSoundDeviceConfig)} {

    auto generator = std::make_unique<synthesizer::SineGenerator>(
        0.2, 1000, nullptr, nullptr);
    sound_device_->SetGenerator(std::move(generator));
}

void PlayerView::Render() {
    RenderWindow();
}

auto PlayerView::WindowName() -> const std::string & { return kWindowName; }

void PlayerView::RenderWindow() {
    auto window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {kWindowPadding, kWindowPadding});
    ImGui::Begin(kWindowName.c_str(), nullptr, window_flags);

    auto button_label = sound_device_->IsPlaying() ? kPauseButtonText : kPlayButtonText;
    if (ImGui::Button(button_label.c_str())) {
        if (sound_device_->IsPlaying()) {
            sound_device_->Pause();
        } else {
            sound_device_->Play();
        }
    }
    ImGui::SameLine();
    ImGui::Text("Queue: %d", sound_device_->GetQueueSize());

    ImGui::End();
    ImGui::PopStyleVar();
}

}
