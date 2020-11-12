#include "node_views.h"

#include <utility>

namespace wave_generator::view::node {

const std::string ConstantGeneratorNodeView::kNodeName = "Constant";

ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : NodeView{position} {}

auto ConstantGeneratorNodeView::GetName() -> const std::string& {
    return kNodeName;
}

const std::string SineWaveGeneratorNodeView::kNodeName = "Constant";

SineWaveGeneratorNodeView::SineWaveGeneratorNodeView(ImVec2 position)
    : NodeView{position}, amplitude_input_node_{"amplitude"} {}

auto SineWaveGeneratorNodeView::GetName() -> const std::string& {
    return kNodeName;
}

auto SineWaveGeneratorNodeView::GetInputViews() -> std::vector<const NodeInputView*> {
    return {&amplitude_input_node_};
}

}  // namespace wave_generator::view::node
