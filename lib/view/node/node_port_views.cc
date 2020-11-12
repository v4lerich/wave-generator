#include "node_port_views.h"

#include <utility>

namespace wave_generator::view::node {

SignalNodeInputView::SignalNodeInputView(const NodeView& parent, std::string name)
    : NodeInputView{parent, std::move(name), true} {}

void SignalNodeInputView::RenderItem(ImDrawList *draw_list) {
    ImGui::BeginGroup();
    ImGui::Text("%s", GetName().c_str());
    ImGui::EndGroup();
}


FloatInputView::FloatInputView(const NodeView& parent, std::string name, ImVec2 range, float default_value)
    : NodeInputView{parent, std::move(name)}, range_{range}, value_{default_value} {}

auto FloatInputView::GetValue() const -> float { return value_; }

void FloatInputView::RenderItem(ImDrawList *draw_list) {
    ImGui::SliderFloat(GetName().c_str(), &value_, range_[0], range_[1]);
}

SignalNodeOutputView::SignalNodeOutputView(const NodeView& parent, std::string name)
    : NodeOutputView{parent, std::move(name)} {}

}