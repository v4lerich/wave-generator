#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <constant_generator.h>

#include <memory>

#include "node_port_views.h"
#include "node_view.h"

namespace wave_generator::view::node {

class SignalGeneratorNodeView : public NodeView {
  public:
    explicit SignalGeneratorNodeView(std::string name, ImVec2 position = {});
    virtual auto CreateGenerator()
        -> std::unique_ptr<synthesizer::SignalGenerator> = 0;
};

class ConstantGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit ConstantGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetOutputViews() -> std::vector<NodeOutputView *> override;
    auto CreateGenerator()
        -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    float value_;
    node::SignalPortOutputView output_node_;
};

class SineWaveGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit SineWaveGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator()
        -> std::unique_ptr<synthesizer::SignalGenerator> override;

  protected:
    auto GetInputViews() -> std::vector<NodeInputView *> override;
    auto GetOutputViews() -> std::vector<NodeOutputView *> override;

  private:
    node::SignalPortInputView frequency_input_port_;
    node::SignalPortInputView amplitude_input_port_;
    node::FloatInputView base_frequency_input_;
    node::FloatInputView base_amplitude_input_;
    node::SignalPortOutputView output_node_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
