#ifndef WAVEGENERATOR_NODE_PORT_VIEW_H
#define WAVEGENERATOR_NODE_PORT_VIEW_H

#include <imgui.h>
#include <imgui_internal.h>
#include <view.h>

#include <memory>
#include <string>
#include <unordered_set>

namespace wave_generator::view::node {

class NodeView;
class NodeInputView;
class NodeOutputView;

class NodeInputView {
  public:
    explicit NodeInputView(const NodeView& parent, std::string name = "Input",
                           bool has_port = false);
    void Render(ImDrawList* draw_list);

    void Connect(NodeOutputView* output);
    void Disconnect();

    auto GetPortPosition() const -> ImVec2;
    auto GetPortSize() const -> ImVec2;
    auto GetPortRect() const -> ImRect;
    auto HasPort() const -> bool;

    virtual auto CanConnect(const NodeOutputView* output) const -> bool;

  protected:
    virtual void RenderItem(ImDrawList* draw_list) = 0;

    auto GetName() const -> const std::string&;

  private:
    static auto GetID() -> int;
    static int counter_id_;

    NodeOutputView* connected_output_{nullptr};
    const NodeView& parent_;
    int id_;
    bool has_port_;
    std::string name_;
    ImVec2 port_position_{};
};

class NodeOutputView {
  public:
    explicit NodeOutputView(const NodeView& parent,
                            std::string name = "Output");
    void Render(ImDrawList* draw_list);
    auto GetPortPosition() const -> ImVec2;
    auto GetPortSize() const -> ImVec2;
    auto GetPortRect() const -> ImRect;

    void Connect(NodeInputView* input);
    void Disconnect(NodeInputView* input);
    auto IsConnecting() const -> bool;

  private:
    std::unordered_set<const NodeInputView *> connected_inputs_{};
    const NodeView& parent_;
    std::string name_;
    ImVec2 position_{};

    bool is_connecting_{};
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_PORT_VIEW_H
