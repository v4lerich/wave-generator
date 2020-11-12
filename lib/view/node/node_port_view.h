#ifndef WAVEGENERATOR_NODE_PORT_VIEW_H
#define WAVEGENERATOR_NODE_PORT_VIEW_H

#include <imgui.h>
#include <view.h>

#include <memory>
#include <string>

namespace wave_generator::view::node {

class NodeView;
class NodeInputView;
class NodeOutputView;

class NodeInputView {
  public:
    explicit NodeInputView(const NodeView& parent, std::string name = "Input", bool has_port = false);
    void Render(ImDrawList *draw_list);

  protected:
    virtual void RenderItem(ImDrawList *draw_list) = 0;
    virtual auto CanConnect(ImVec2 position, std::shared_ptr<NodeOutputView> connecting_node) -> bool;
    virtual void Connect(std::shared_ptr<NodeOutputView> connecting_node);

    auto GetName() const -> const std::string&;

  private:
    const NodeView& parent_;
    std::string name_;
    bool has_port_;
};

class NodeOutputView {
  public:
    explicit NodeOutputView(const NodeView& parent, std::string name = "Output");
    void Render(ImDrawList *draw_list);

  private:
    const NodeView& parent_;
    std::string name_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_PORT_VIEW_H
