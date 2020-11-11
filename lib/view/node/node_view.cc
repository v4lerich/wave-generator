#include "node_view.h"

#include <imgui_internal.h>

namespace wave_generator::view::node {

const ImRect NodeView::kPadding = ImRect(10, 10, 10, 10);
const ImColor NodeView::kBackgroundColor = ImColor(75, 75, 75);
const ImColor NodeView::kBorderColor = ImColor(100, 100, 100);

int NodeView::id_counter_ = 0;

NodeView::NodeView(ImVec2 position)
    : position_{position}, size_{}, id_{GenerateId()} {}

void NodeView::Render(ImDrawList *draw_list, ImVec2 offset) {
    const auto start_position = offset + position_;
    const auto internal_start_position = start_position + kPadding.GetTL();

    ImGui::PushID(id_);

    ImGui::PushItemWidth(120);

    draw_list->ChannelsSetCurrent(1);
    ImGui::SetCursorScreenPos(internal_start_position);
    ImGui::BeginGroup();
    ImGui::Text("%s", GetName().c_str());
    static float x = 0;
    ImGui::InputFloat("float", &x);
    ImGui::EndGroup();

    size_ = ImGui::GetItemRectSize();
    const auto internal_end_position = internal_start_position + size_;
    const auto end_position = internal_end_position + kPadding.GetBR();

    draw_list->ChannelsSetCurrent(0);
    draw_list->AddRectFilled(start_position, end_position, kBackgroundColor, 4);
    draw_list->AddRect(start_position, end_position, kBorderColor, 4);

    ImGui::PopItemWidth();
    ImGui::PopID();
}

auto NodeView::GetID() -> int { return id_; }

auto NodeView::GenerateId() -> int { return id_counter_; }

}  // namespace wave_generator::view::node