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
    .samples = 4096,
};

PlayerView::PlayerView()
    : sound_device_{std::make_shared<model::SoundDevice>(kSoundDeviceConfig)} {

    auto generator = std::make_unique<synthesizer::PulseGenerator>(
        0.1,
        0.2, 100, nullptr, nullptr);
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

    ImGui::End();
    ImGui::PopStyleVar();
}

}
