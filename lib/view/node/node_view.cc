#include "node_view.h"

#include <imgui_internal.h>

#include <utility>

namespace wave_generator::view::node {

static const ImRect kPadding = ImRect(10, 10, 10, 10);
static const ImColor kBackgroundColor = ImColor(75, 75, 75);
static const ImColor kBorderColor = ImColor(100, 100, 100);
static const ImColor kHeaderColor = ImColor(200, 0, 0);
static const float kHeaderPadding = 5.0F;
static const float kRounding = 5.0F;

int NodeView::id_counter_ = 0;

NodeView::NodeView(std::string name, ImVec2 position)
    : position_{position}, size_{}, id_{GenerateId()}, name_{std::move(name)} {}

void NodeView::Render(ImDrawList* draw_list, ImVec2 offset) {
    offset_ = offset;

    ImGui::PushID(id_);
    ImGui::PushItemWidth(200.0F);

    RenderNodeComponents(draw_list, GetInnerRect().Min);

    RenderInteractNode(draw_list, GetOuterRect().Min);

    const auto outer_rect = GetOuterRect();
    draw_list->ChannelsSetCurrent(0);
    draw_list->AddRectFilled(outer_rect.Min, outer_rect.Max, kBackgroundColor,
                             kRounding);
    draw_list->AddRectFilled(outer_rect.Min, {outer_rect.Max.x, header_rect_.Max.y},
                             kHeaderColor, kRounding);
    draw_list->AddRect(outer_rect.Min, outer_rect.Max, kBorderColor, kRounding);

    ImGui::PopItemWidth();
    ImGui::PopID();
}

auto NodeView::GetID() -> int { return id_; }

auto NodeView::IsActive() -> bool { return is_active_; }

auto NodeView::GetPadding() -> ImRect { return kPadding; }

auto NodeView::GetInnerSize() const -> ImVec2 { return size_; }

auto NodeView::GetOuterSize() const -> ImVec2 { return size_ + kPadding.Min + kPadding.Max; }

auto NodeView::GenerateId() -> int { return id_counter_++; }

auto NodeView::GetInputViews() -> std::vector<NodeInputView*> { return {}; }

auto NodeView::GetOutputViews() -> std::vector<NodeOutputView*> { return {}; }

auto NodeView::GetInnerRect() const -> ImRect {
    auto start_position = offset_ + position_ + kPadding.GetTL();
    auto end_position = start_position + GetInnerSize();
    return {start_position, end_position};
}

auto NodeView::GetOuterRect() const -> ImRect {
    auto inner_rect = GetInnerRect();
    return {inner_rect.Min - kPadding.GetTL(),
            inner_rect.Max + kPadding.GetBR()};
}

void NodeView::RenderNodeComponents(ImDrawList* draw_list, ImVec2 position) {
    draw_list->ChannelsSetCurrent(1);
    ImGui::SetCursorScreenPos(position);

    ImGui::BeginGroup();
    ImGui::Text("%s", name_.c_str());
    header_rect_.Min = ImGui::GetItemRectMin();
    ImGui::Dummy(ImVec2(0.0F, kHeaderPadding));
    header_rect_.Max = ImGui::GetItemRectMax();

    for (const auto& input : GetInputViews()) {
        input->Render(draw_list);
    }
    for (const auto& output : GetOutputViews()) {
        output->Render(draw_list);
    }
    ImGui::EndGroup();
    size_ = ImGui::GetItemRectSize();
}

void NodeView::RenderInteractNode(ImDrawList* draw_list, ImVec2 position) {
    draw_list->ChannelsSetCurrent(0);
    ImGui::SetCursorScreenPos(position);

    ImGui::InvisibleButton("interact_node", GetOuterSize());
    is_active_ = ImGui::IsItemActive();
}

void NodeView::Move(ImVec2 delta) {
    position_ += delta;
    position_.x = std::max(position_.x, 0.0F);
    position_.y = std::max(position_.y, 0.0F);

}

}  // namespace wave_generator::view::node