#include "node_view.h"

#include <imgui_internal.h>

#include <algorithm>
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

    const auto outer_rect = GetOuterRect();
    draw_list->ChannelsSetCurrent(background_channel_);
    draw_list->AddRectFilled(outer_rect.Min, outer_rect.Max, kBackgroundColor, kRounding);
    draw_list->AddRectFilled(outer_rect.Min, {outer_rect.Max.x, header_rect_.Max.y}, kHeaderColor,
                             kRounding);
    draw_list->AddRect(outer_rect.Min, outer_rect.Max, kBorderColor, kRounding);

    RenderInteractNode(draw_list, GetOuterRect().Min);

    ImGui::PopItemWidth();
    ImGui::PopID();

    is_initialized_ = true;
}

auto NodeView::GetID() const -> int { return id_; }

auto NodeView::IsActive() const -> bool { return is_active_; }

auto NodeView::IsConnecting() -> bool { return connecting_output_ != nullptr; }

auto NodeView::GetPadding() -> ImRect { return kPadding; }

auto NodeView::GetInnerSize() const -> ImVec2 { return size_; }

auto NodeView::GetOuterSize() const -> ImVec2 { return size_ + kPadding.Min + kPadding.Max; }

auto NodeView::GenerateId() -> int { return id_counter_++; }

auto NodeView::GetInputViews() -> std::list<NodeInputView*> { return {}; }

auto NodeView::GetOutputViews() -> std::list<NodeOutputView*> { return {}; }

auto NodeView::GetInnerRect() const -> ImRect {
    auto start_position = offset_ + position_ + kPadding.GetTL();
    auto end_position = start_position + GetInnerSize();
    return {start_position, end_position};
}

auto NodeView::GetOuterRect() const -> ImRect {
    auto inner_rect = GetInnerRect();
    return {inner_rect.Min - kPadding.GetTL(), inner_rect.Max + kPadding.GetBR()};
}

void NodeView::RenderNodeComponents(ImDrawList* draw_list, ImVec2 position) {
    draw_list->ChannelsSetCurrent(foreground_channel_);
    ImGui::SetCursorScreenPos(position);

    ImGui::BeginGroup();
    BeginRender();
    ImGui::Text("%s", name_.c_str());
    header_rect_.Min = ImGui::GetItemRectMin();
    ImGui::Dummy(ImVec2(0.0F, kHeaderPadding));
    header_rect_.Max = ImGui::GetItemRectMax();

    for (const auto& input : GetInputViews()) {
        input->SetChannels(foreground_channel_, background_channel_);
        input->Render(draw_list);
    }

    connecting_output_ = nullptr;
    for (const auto& output : GetOutputViews()) {
        output->SetChannels(foreground_channel_, background_channel_);
        output->Render(draw_list);

        if (output->IsConnecting()) {
            connecting_output_ = output;
        }
    }
    EndRender();
    ImGui::EndGroup();

    if (!is_initialized_) {
        size_ = ImGui::GetItemRectSize();
    } else {
        size_.y = ImGui::GetItemRectSize().y;
    }
}

void NodeView::RenderInteractNode(ImDrawList* draw_list, ImVec2 position) {
    draw_list->ChannelsSetCurrent(0);
    ImGui::SetCursorScreenPos(position);

    ImGui::InvisibleButton("interact_node", GetOuterSize());
    is_active_ = ImGui::IsItemActive();
    is_context_open_ = ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
}

void NodeView::Move(ImVec2 delta) {
    position_ += delta;
    position_.x = std::max(position_.x, 0.0F);
    position_.y = std::max(position_.y, 0.0F);
}

auto NodeView::GetConnectingOutput() -> NodeOutputView* { return connecting_output_; }

auto NodeView::GetInput(ImVec2 position) -> NodeInputView* {
    auto inputs = GetInputViews();
    auto found_input = std::find_if(std::begin(inputs), std::end(inputs), [&](const auto& input) {
        return input->HasPort() && input->GetPortRect().Contains(position);
    });
    return found_input == std::end(inputs) ? nullptr : *found_input;
}

void NodeView::Disconnect() {
    auto inputs = GetInputViews();
    for (auto& input : inputs) {
        input->Disconnect();
    }

    auto outputs = GetOutputViews();
    for (auto& output : outputs) {
        output->DisconnectAll();
    }
}

auto NodeView::IsContextOpen() const -> bool { return is_context_open_; }

auto NodeView::IsDeletable() const -> bool { return true; }

void NodeView::SetChannels(int foreground_channel, int background_channel) {
    foreground_channel_ = foreground_channel;
    background_channel_ = background_channel;
}

void NodeView::BeginRender() {}

void NodeView::EndRender() {}

auto NodeView::IsTopologyChanged() -> bool {
    auto inputs = GetInputViews();
    auto is_input_changed =
        std::any_of(std::begin(inputs), std::end(inputs),
                    [](const auto& input) { return input->IsTopologyChanged(); });

    auto outputs = GetOutputViews();
    auto is_output_changed =
        std::any_of(std::begin(outputs), std::end(outputs),
                    [](const auto& output) { return output->IsTopologyChanged(); });

    return is_input_changed || is_output_changed;
}

}  // namespace wave_generator::view::node