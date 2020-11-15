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
                               float default_value, Type type)
    : NodeInputView{parent, std::move(name)}, range_{range}, value_{default_value}, type_{type} {}

auto FloatInputView::GetValue() const -> float { return value_; }

void FloatInputView::RenderItem(ImDrawList* draw_list) {
    ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    if (type_ == Type::Logarithmic) {
        flags |= ImGuiSliderFlags_Logarithmic;
    }
    ImGui::SliderFloat("", &value_, range_[0], range_[1], (GetName() + ": %.3f").c_str(), flags);
}

SwitchInputView::SwitchInputView(const NodeView* parent, std::string name,
                                 std::vector<std::string> cases)
    : NodeInputView{parent, std::move(name), false}, cases_(cases), case_{cases.front()} {}

auto SwitchInputView::GetCase() const -> std::string { return case_; }

void SwitchInputView::RenderItem(ImDrawList* draw_list) {
    has_changed_ = false;
    ImGui::BeginGroup();
    if (ImGui::BeginCombo("", (GetName() + ": " + case_).c_str())) {
        for (const auto& case_element : cases_) {
            if (ImGui::Selectable(case_element.c_str(), case_element == case_)) {
                has_changed_ = case_ != case_element;
                case_ = case_element;
            }
            if (case_element == case_) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::EndGroup();
}

bool SwitchInputView::HasChanged() { return has_changed_; }

SignalPortOutputView::SignalPortOutputView(const NodeView* parent, std::string name)
    : NodeOutputView{parent, std::move(name)} {}

}  // namespace wave_generator::view::node