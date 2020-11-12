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

    const auto start_position = offset_ + position_;
    const auto internal_start_position = start_position + kPadding.GetTL();

    ImGui::PushID(id_);

    ImGui::PushItemWidth(200.0F);

    draw_list->ChannelsSetCurrent(1);
    ImGui::SetCursorScreenPos(internal_start_position);

    ImGui::BeginGroup();
    ImGui::Text("%s", name_.c_str());
    ImGui::Dummy(ImVec2(0.0F, kHeaderPadding));
    auto header_max_y = ImGui::GetItemRectMax().y;

    for (const auto& input : GetInputViews()) {
        input->Render(draw_list);
    }
    for (const auto& output : GetOutputViews()) {
        output->Render(draw_list);
    }

    ImGui::EndGroup();

    size_ = ImGui::GetItemRectSize();
    const auto internal_end_position = internal_start_position + size_;
    const auto end_position = internal_end_position + kPadding.GetBR();

    draw_list->ChannelsSetCurrent(0);
    draw_list->AddRectFilled(start_position, end_position, kBackgroundColor,
                             kRounding);
    draw_list->AddRectFilled(start_position, {end_position.x, header_max_y},
                             kHeaderColor, kRounding);

    draw_list->AddRect(start_position, end_position, kBorderColor, kRounding);

    ImGui::PopItemWidth();
    ImGui::PopID();
}

auto NodeView::GetID() -> int { return id_; }

auto NodeView::GetSize() const -> ImVec2 { return size_; }

auto NodeView::GetPadding() -> ImRect { return kPadding; }

auto NodeView::GenerateId() -> int { return id_counter_; }

auto NodeView::GetInputViews() -> std::vector<NodeInputView*> { return {}; }

auto NodeView::GetOutputViews() -> std::vector<NodeOutputView*> { return {}; }

auto NodeView::GetInnerRect() const -> ImRect {
    auto start_position = offset_ + position_ + kPadding.GetTL();
    auto end_position = start_position + size_;
    return {start_position, end_position};
}

auto NodeView::GetOuterRect() const -> ImRect {
    auto inner_rect = GetInnerRect();
    return {inner_rect.Min - kPadding.GetTL(),
            inner_rect.Max + kPadding.GetBR()};
}

}  // namespace wave_generator::view::node