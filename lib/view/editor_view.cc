#include "editor_view.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace wave_generator::view {

const std::string EditorView::kWindowName = "editor";

void EditorView::Render() {
    RenderWindow();
}

const std::string& EditorView::WindowName() { return kWindowName; }

void EditorView::RenderWindow() {
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin(EditorView::WindowName().c_str(), nullptr, window_flags))
        return;

    auto offset = ImGui::GetCursorScreenPos();

    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    for (int i = 0; i < 5; i++) {
        ImGui::PushID(i);

        ImGui::SetCursorPos(offset + ImVec2(20 * i, 20 * i));
        ImGui::BeginGroup();
        ImGui::Text("Hello???");
        ImGui::EndGroup();

        ImGui::PopID();
    }
    draw_list->ChannelsMerge();

    ImGui::End();
}

}  // namespace wave_generator::view
