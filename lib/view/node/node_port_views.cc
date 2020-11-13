#include "node_port_views.h"

#include <utility>

#include "node_views.h"

namespace wave_generator::view::node {

SignalPortInputView::SignalPortInputView(const NodeView* parent, std::string name)
    : NodeInputView{parent, std::move(name), true} {}

void SignalPortInputView::RenderItem(ImDrawList* draw_list) {
    ImGui::BeginGroup();
    ImGui::Text("%s", GetName().c_str());
    ImGui::EndGroup();
}

auto SignalPortInputView::GetConnectedSignalNode() const -> const SignalGeneratorNodeView* {
    return dynamic_cast<const SignalGeneratorNodeView*>(GetConnectedNode());
}

bool SignalPortInputView::CanConnect(const NodeOutputView* output) const {
    return dynamic_cast<const SignalPortOutputView*>(output) != nullptr;
}

FloatInputView::FloatInputView(const NodeView* parent, std::string name, ImVec2 range,
                               float default_value)
    : NodeInputView{parent, std::move(name)}, range_{range}, value_{default_value} {}

auto FloatInputView::GetValue() const -> float { return value_; }

void FloatInputView::RenderItem(ImDrawList* draw_list) {
    ImGui::SliderFloat("", &value_, range_[0], range_[1], (GetName() + ": %.3f").c_str());
}

SignalPortOutputView::SignalPortOutputView(const NodeView* parent, std::string name)
    : NodeOutputView{parent, std::move(name)} {}

}  // namespace wave_generator::view::node