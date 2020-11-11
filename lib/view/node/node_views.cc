#include "node_views.h"

#include <utility>

namespace wave_generator::view::node {

const std::string ConstantGeneratorNodeView::kNodeName = "Constant";

ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : NodeView(position) {}

auto ConstantGeneratorNodeView::GetName() -> const std::string& {
    return kNodeName;
}
}  // namespace wave_generator::view::node
