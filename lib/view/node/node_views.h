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
    virtual auto CreateGenerator() -> std::unique_ptr<synthesizer::SignalGenerator> = 0;
};

class ConstantGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit ConstantGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetOutputViews() -> std::vector<NodeOutputView *> override;
    auto CreateGenerator() -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    float value_;
    node::NodeOutputView output_node_;
};

class SineWaveGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit SineWaveGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator() -> std::unique_ptr<synthesizer::SignalGenerator> override;

  protected:
    auto GetInputViews() -> std::vector<NodeInputView*> override;

  private:
    node::SignalNodeInputView amplitude_input_node_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
