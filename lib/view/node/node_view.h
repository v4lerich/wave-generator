#ifndef WAVEGENERATOR_NODE_VIEW_H
#define WAVEGENERATOR_NODE_VIEW_H

#include <imgui.h>
#include <imgui_internal.h>
#include <view.h>

#include <string>
#include <vector>

#include "node_port_view.h"

namespace wave_generator::view::node {

class NodeView {
    friend class NodeInputView;
    friend class NodeOutputView;

  public:
    explicit NodeView(std::string name, ImVec2 position = {});

    void Render(ImDrawList *draw_list, ImVec2 offset = {});

    auto GetID() const -> int;
    auto IsActive() const -> bool;
    auto IsContextOpen() const -> bool;

    auto IsConnecting() -> bool;
    void Disconnect();
    auto GetConnectingOutput() -> NodeOutputView *;
    auto GetInput(ImVec2 position) -> NodeInputView *;

    void Move(ImVec2 delta);

    auto GetInnerRect() const -> ImRect;
    auto GetOuterRect() const -> ImRect;

  protected:
    virtual auto GetInputViews() -> std::vector<NodeInputView *>;
    virtual auto GetOutputViews() -> std::vector<NodeOutputView *>;

  private:
    static auto GenerateId() -> int;
    static auto GetPadding() -> ImRect;

    auto GetInnerSize() const -> ImVec2;
    auto GetOuterSize() const -> ImVec2;

    void RenderNodeComponents(ImDrawList *draw_list, ImVec2 position);
    void RenderInteractNode(ImDrawList *draw_list, ImVec2 position);

    static int id_counter_;

    ImVec2 offset_{};
    ImVec2 position_{};
    ImVec2 size_{};
    ImRect header_rect_{};
    bool is_active_{};
    bool is_initialized_{};
    bool is_context_open_{};
    NodeOutputView* connecting_output_{};

    std::string name_;
    int id_;
};



}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEW_H
