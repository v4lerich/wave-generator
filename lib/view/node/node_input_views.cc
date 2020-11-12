#include "node_input_views.h"

#include <utility>

namespace wave_generator::view::node {

SignalNodeInputView::SignalNodeInputView(std::string name)
    : NodeInputView(std::move(name)) {}

void SignalNodeInputView::RenderItem(ImDrawList *draw_list) const {
    ImGui::BeginGroup();
    ImGui::Text("%s", GetName().c_str());
    ImGui::EndGroup();
}

}