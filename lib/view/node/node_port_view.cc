#include "node_port_view.h"

#include <imgui_internal.h>

#include <utility>

#include "node_view.h"

namespace wave_generator::view::node {

static const ImColor kInputPortBackgroundColor = ImColor(150, 150, 150);
static const ImColor kInputPortBorderColor = ImColor(170, 170, 170);
static const float kPortRadius = 6.0F;

int NodeInputView::counter_id_ = 0;

static void DrawPort(ImDrawList* draw_list, ImVec2 position) {
    draw_list->ChannelsSetCurrent(1);
    draw_list->AddCircleFilled(position, kPortRadius, kInputPortBackgroundColor);
    draw_list->AddCircle(position, kPortRadius, kInputPortBorderColor);
}

NodeInputView::NodeInputView(const NodeView& parent, std::string name,
                             bool has_port)
    : parent_{parent}, id_{GetID()}, name_{std::move(name)}, has_port_{has_port} {}

void NodeInputView::Render(ImDrawList* draw_list) {
    ImGui::BeginGroup();
    ImGui::PushID(id_);
    RenderItem(draw_list);
    ImGui::PopID();
    ImGui::EndGroup();

    if (has_port_) {
        auto item_rect =
            ImRect{ImGui::GetItemRectMin(), ImGui::GetItemRectMax()};
        auto port_position =
            ImVec2{parent_.GetOuterRect().Min.x, item_rect.GetCenter().y};
        DrawPort(draw_list, port_position);
    }
}

auto NodeInputView::CanConnect(ImVec2 position,
                               std::shared_ptr<NodeOutputView> connecting_node)
    -> bool {
    return false;
}

void NodeInputView::Connect(std::shared_ptr<NodeOutputView> connecting_node) {}

auto NodeInputView::GetName() const -> const std::string& { return name_; }

auto NodeInputView::GetID() -> int { return counter_id_++; }

NodeOutputView::NodeOutputView(const NodeView& parent, std::string name)
    : parent_{parent}, name_{std::move(name)} {}

void NodeOutputView::Render(ImDrawList *draw_list) {
    ImGui::BeginGroup();
    ImGui::Text("%s", name_.c_str());
    ImGui::EndGroup();

    auto item_rect = ImRect{ImGui::GetItemRectMin(), ImGui::GetItemRectMax()};
    auto port_position =
        ImVec2{parent_.GetOuterRect().Max.x, item_rect.GetCenter().y};
    DrawPort(draw_list, port_position);
}

}  // namespace wave_generator::view::node
