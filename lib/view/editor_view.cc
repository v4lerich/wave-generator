#include "editor_view.h"

#include <algorithm>
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
                                    ImGuiWindowFlags_HorizontalScrollbar |
                                    ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin(EditorView::WindowName().c_str(), nullptr,
                      window_flags)) {
        return;
    }

    auto offset = ImGui::GetCursorScreenPos() + scrolling_offset_;
    auto draw_list = ImGui::GetWindowDrawList();

    draw_list->ChannelsSplit(2);
    for (auto& node : nodes_) {
        node->Render(draw_list, offset);

        if (node->IsActive()) {
            auto move_delta = ImGui::GetIO().MouseDelta;
            auto moved_rect = node->GetOuterRect();
            moved_rect.Translate(move_delta);

            bool is_overlap = std::any_of(std::begin(nodes_), std::end(nodes_),
                [&](const auto& other_node) { return other_node->GetID() != node->GetID() && moved_rect.Overlaps(other_node->GetOuterRect()); }
            );

            if (!is_overlap) {
                node->Move(move_delta);
            }
        }
    }
    draw_list->ChannelsMerge();

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        scrolling_offset_ += ImGui::GetIO().MouseDelta;
        scrolling_offset_.x = std::max(scrolling_offset_.x, 0.0F);
        scrolling_offset_.y = std::max(scrolling_offset_.y, 0.0F);
    }
    ImGui::End();
}

}  // namespace wave_generator::view
