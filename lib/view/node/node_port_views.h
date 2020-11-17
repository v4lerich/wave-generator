#ifndef WAVEGENERATOR_NODE_PORT_VIEWS_H
#define WAVEGENERATOR_NODE_PORT_VIEWS_H

#include <signal_generator.h>

#include <vector>

#include "node_view.h"

namespace wave_generator::view::node {

class SignalGeneratorNodeView;

class SignalPortInputView final : public NodeInputView {
  public:
    explicit SignalPortInputView(const NodeView* parent, std::string name);
    auto GetConnectedSignalNode() const -> const SignalGeneratorNodeView*;
    auto CreateConnectedGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator>;

    bool CanConnect(const NodeOutputView* output) const override;

  protected:
    void RenderItem(ImDrawList* draw_list) override;
};

class FloatInputView final : public NodeInputView {
  public:
    enum class Type { Logarithmic, Linear };
    explicit FloatInputView(const NodeView* parent, std::string name, ImVec2 range = {0.0F, 1.0F},
                            float default_value = 1.0F, Type type = Type::Linear);
    auto GetValue() const -> float;
    void SetValue(float value);

  protected:
    void RenderItem(ImDrawList* draw_list) override;

  private:
    ImVec2 range_;
    float value_;
    Type type_;
};

class IntInputView final : public NodeInputView {
  public:
    using Range = std::pair<int, int>;
    explicit IntInputView(const NodeView* parent, std::string name, Range range = {1, 1},
                          int default_value = 1);
    auto GetValue() const -> int;

  protected:
    void RenderItem(ImDrawList* draw_list) override;

  private:
    Range range_;
    int value_;
};

class SwitchInputView final : public NodeInputView {
  public:
    explicit SwitchInputView(const NodeView* parent, std::string name,
                             std::vector<std::string> cases);
    auto GetCase() const -> std::string;

    bool HasChanged();

  protected:
    void RenderItem(ImDrawList* draw_list) override;

  private:
    std::vector<std::string> cases_;
    std::string case_;
    bool has_changed_{};
};

class SignalPortOutputView final : public NodeOutputView {
  public:
    explicit SignalPortOutputView(const NodeView* parent, std::string name = "Output");
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_PORT_VIEWS_H
