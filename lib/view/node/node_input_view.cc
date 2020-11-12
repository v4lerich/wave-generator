#include "node_input_view.h"

#include <utility>

namespace wave_generator::view::node {

NodeInputView::NodeInputView(std::string name)
    : name_{std::move(name)} {
}

void NodeInputView::Render(ImDrawList *draw_list) const {
    ImGui::BeginGroup();
    RenderItem(draw_list);
    ImGui::EndGroup();
}

auto NodeInputView::GetName() const -> const std::string& { return name_; }

}  // namespace wave_generator::view::node
