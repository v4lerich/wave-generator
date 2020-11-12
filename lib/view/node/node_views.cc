#include "node_views.h"

#include <utility>

namespace wave_generator::view::node {

SignalGeneratorNodeView::SignalGeneratorNodeView(std::string name,
                                                 ImVec2 position)
    : NodeView(std::move(name), position) {}


ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Constant", position}, value_{1.0F}, output_node_{*this} {}

auto ConstantGeneratorNodeView::GetOutputViews() -> std::vector<NodeOutputView*> {
    return {&output_node_};
}

auto ConstantGeneratorNodeView::CreateGenerator()
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::ConstantGenerator>(value_);
}


SineWaveGeneratorNodeView::SineWaveGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Sina", position}, amplitude_input_node_{*this, "amplitude"} {}

auto SineWaveGeneratorNodeView::GetInputViews() -> std::vector<NodeInputView*> {
    return {&amplitude_input_node_};
}

auto SineWaveGeneratorNodeView::CreateGenerator()
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return nullptr;
}

}  // namespace wave_generator::view::node
