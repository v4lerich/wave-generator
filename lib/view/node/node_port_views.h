#ifndef WAVEGENERATOR_NODE_PORT_VIEWS_H
#define WAVEGENERATOR_NODE_PORT_VIEWS_H

#include "node_view.h"

namespace wave_generator::view::node {

class SignalGeneratorNodeView;

class SignalPortInputView final : public NodeInputView {
  public:
    explicit SignalPortInputView(const NodeView* parent, std::string name);
    auto GetConnectedSignalNode() const -> const SignalGeneratorNodeView*;

    bool CanConnect(const NodeOutputView* output) const override;

  protected:
    void RenderItem(ImDrawList* draw_list) override;
};

class FloatInputView final : public NodeInputView {
  public:
    explicit FloatInputView(const NodeView* parent, std::string name, ImVec2 range = {0.0F, 1.0F},
                            float default_value = 1.0F);
    auto GetValue() const -> float;

  protected:
    void RenderItem(ImDrawList* draw_list) override;

  private:
    ImVec2 range_;
    float value_;
};

class SignalPortOutputView final : public NodeOutputView {
  public:
    explicit SignalPortOutputView(const NodeView* parent, std::string name = "Output");
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_PORT_VIEWS_H
