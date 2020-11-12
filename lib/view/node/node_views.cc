#include "node_views.h"

#include <utility>

namespace wave_generator::view::node {

SignalGeneratorNodeView::SignalGeneratorNodeView(std::string name,
                                                 ImVec2 position)
    : NodeView(std::move(name), position) {}

ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Constant", position},
      value_{1.0F},
      output_node_{*this} {}

auto ConstantGeneratorNodeView::GetOutputViews()
    -> std::vector<NodeOutputView*> {
    return {&output_node_};
}

auto ConstantGeneratorNodeView::CreateGenerator()
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::ConstantGenerator>(value_);
}

SineWaveGeneratorNodeView::SineWaveGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Sine Wave", position},
      amplitude_input_port_{*this, "Amplitude"},
      frequency_input_port_{*this, "Frequency"},
      base_amplitude_input_{*this, "Base amplitude"},
      base_frequency_input_{*this, "Base frequency"},
      output_node_{*this}
      {}

auto SineWaveGeneratorNodeView::GetInputViews() -> std::vector<NodeInputView*> {
    return {
        &base_amplitude_input_,
        &amplitude_input_port_,
        &base_frequency_input_,
        &frequency_input_port_,
    };
}

auto SineWaveGeneratorNodeView::CreateGenerator()
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return nullptr;
}
auto SineWaveGeneratorNodeView::GetOutputViews()
    -> std::vector<NodeOutputView *> {
    return {&output_node_};
}

}  // namespace wave_generator::view::node
