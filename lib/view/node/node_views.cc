#include "node_views.h"

#include <constant_generator.h>

#include <utility>

namespace wave_generator::view::node {

SignalGeneratorNodeView::SignalGeneratorNodeView(std::string name,
                                                 ImVec2 position)
    : NodeView(std::move(name), position) {}

ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Constant", position},
      constant_input_{*this, "Value"},
      output_node_{*this} {}

auto ConstantGeneratorNodeView::GetOutputViews()
    -> std::vector<NodeOutputView*> {
    return {&output_node_};
}

auto ConstantGeneratorNodeView::GetInputViews()
    -> std::vector<NodeInputView *> {
    return {&constant_input_};
}

auto ConstantGeneratorNodeView::CreateGenerator()
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::ConstantGenerator>(constant_input_.GetValue());
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

NodeViewFactoryStorage::NodeViewFactoryEntry::NodeViewFactoryEntry(
    std::string name,
    Constructor constructor)
    : name_{std::move(name)}, constructor_{std::move(constructor)} {}

const std::string &NodeViewFactoryStorage::NodeViewFactoryEntry::GetName() const {
    return name_;
}

auto NodeViewFactoryStorage::NodeViewFactoryEntry::Construct(ImVec2 position) const -> NodeView * {
    return constructor_(position);
}

auto NodeViewFactoryStorage::GetFactories() -> const std::vector<NodeViewFactoryEntry> & {
    return factories_;
}

void NodeViewFactoryStorage::RegisterFactory(
    const std::string& name, const NodeViewFactoryStorage::Constructor& constructor) {
    factories_.emplace_back(name, constructor);
}

}  // namespace wave_generator::view::node
