#include "editor_view.h"

#include <imgui.h>
#include <node/node_views.h>

namespace wave_generator::view {

const std::string EditorView::kWindowName = "editor";

EditorView::EditorView() {
    auto test_node =
        std::make_shared<node::SineWaveGeneratorNodeView>(ImVec2{0, 0});
    auto test_node2 =
        std::make_shared<node::SineWaveGeneratorNodeView>(ImVec2{300, 0});
    auto test_node3 =
        std::make_shared<node::ConstantGeneratorNodeView>(ImVec2{200, 200});
    nodes_.push_back(test_node);
    nodes_.push_back(test_node2);
    nodes_.push_back(test_node3);
}

void EditorView::Render() { RenderWindow(); }

const std::string& EditorView::WindowName() { return kWindowName; }

void EditorView::RenderWindow() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin(EditorView::WindowName().c_str(), nullptr,
                      window_flags)) {
        return;
    }

    auto offset = ImGui::GetCursorScreenPos();
    auto draw_list = ImGui::GetWindowDrawList();

    draw_list->ChannelsSplit(2);

    for (auto& node : nodes_) {
        node->Render(draw_list, offset);
    }

    draw_list->ChannelsMerge();

    ImGui::End();
}

}  // namespace wave_generator::view
