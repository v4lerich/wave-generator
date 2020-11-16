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
    explicit NodeInputView(const NodeView* parent, std::string name = "Input",
                           bool has_port = false);
    virtual ~NodeInputView() = default;

    void SetChannels(int foreground_channel, int background_channel);
    void Render(ImDrawList* draw_list);

    void Connect(NodeOutputView* output);
    void Disconnect();

    auto IsTopologyChanged() const -> bool;
    auto GetConnectedNode() const -> const NodeView*;
    auto GetPortPosition() const -> ImVec2;
    auto GetPortSize() const -> ImVec2;
    auto GetPortRect() const -> ImRect;
    auto HasPort() const -> bool;

    virtual auto CanConnect(const NodeOutputView* output) const -> bool;

  protected:
    virtual void RenderItem(ImDrawList* draw_list) = 0;

    auto GetName() const -> const std::string&;

    bool is_topology_changed_{};
  private:
    static auto GetID() -> int;
    static int counter_id_;

    NodeOutputView* connected_output_{nullptr};
    const NodeView* parent_;
    int id_;
    bool has_port_;
    std::string name_;
    ImVec2 port_position_{};

    int foreground_channel_{};
    int background_channel_{};
};

class NodeOutputView {
  public:
    explicit NodeOutputView(const NodeView* parent, std::string name = "Output");
    virtual ~NodeOutputView() = default;

    void SetChannels(int foreground_channel, int background_channel);
    void Render(ImDrawList* draw_list);

    auto GetNode() const -> const NodeView*;
    auto GetPortPosition() const -> ImVec2;
    auto GetPortSize() const -> ImVec2;
    auto GetPortRect() const -> ImRect;

    auto IsTopologyChanged() const -> bool;
    void Connect(NodeInputView* input);
    void Disconnect(NodeInputView* input);
    void DisconnectAll();
    auto IsConnecting() const -> bool;

  private:
    std::unordered_set<NodeInputView*> connected_inputs_{};
    const NodeView* parent_;
    std::string name_;
    ImVec2 position_{};

    int foreground_channel_{};
    int background_channel_{};

    bool is_connecting_{};
    bool is_topology_changed_{};
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_PORT_VIEW_H
