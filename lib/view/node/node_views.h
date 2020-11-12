#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <constant_generator.h>

#include <memory>

#include "node_input_views.h"
#include "node_view.h"

namespace wave_generator::view::node {

class ConstantGeneratorNodeView final : public NodeView {
  public:
    explicit ConstantGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetName() -> const std::string& override;

  private:
    static const std::string kNodeName;
};

class SineWaveGeneratorNodeView final : public NodeView {
  public:
    explicit SineWaveGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetName() -> const std::string & override;
    auto GetInputViews() -> std::vector<const NodeInputView*> override;

  private:
    static const std::string kNodeName;
    node::SignalNodeInputView amplitude_input_node_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
