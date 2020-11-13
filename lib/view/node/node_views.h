#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <signal_generator.h>

#include <functional>
#include <memory>

#include "node_port_views.h"
#include "node_view.h"

namespace wave_generator::view::node {

class NodeViewFactoryStorage {
  public:
    using Constructor = std::function<NodeView *(ImVec2)>;

    class NodeViewFactoryEntry {
      public:
        NodeViewFactoryEntry(std::string name, Constructor constructor);
        auto GetName() const -> const std::string&;
        auto Construct(ImVec2 position) const -> NodeView *;

      private:
        std::string name_;
        Constructor constructor_;
    };

    void RegisterFactory(const std::string& name, const Constructor& constructor);
    auto GetFactories() -> const std::vector<NodeViewFactoryEntry>&;

  private:
    std::vector<NodeViewFactoryEntry> factories_;
};

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
    auto GetInputViews() -> std::vector<NodeInputView *> override;
    auto CreateGenerator()
        -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    node::FloatInputView constant_input_;
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
