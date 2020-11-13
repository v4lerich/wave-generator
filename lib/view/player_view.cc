#include "player_view.h"

#include <imgui.h>

namespace wave_generator::view {

static const std::string kWindowName = "player";
static constexpr float kWindowHeight = 20.0;

PlayerView::PlayerView() {}

void PlayerView::Render() {
    RenderWindow();
}

auto PlayerView::WindowName() -> const std::string & { return kWindowName; }

void PlayerView::RenderWindow() {
    auto window_flags = ImGuiWindowFlags_NoResize;
    ImGui::Begin(kWindowName.c_str(), nullptr, window_flags);
    ImGui::Button(u8"fd");

    ImGui::End();
}
auto PlayerView::IsPlaying() -> bool {
    return is_playing_;
}

}
