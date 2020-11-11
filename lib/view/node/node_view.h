#ifndef WAVEGENERATOR_NODE_VIEW_H
#define WAVEGENERATOR_NODE_VIEW_H

#include <imgui.h>
#include <imgui_internal.h>
#include <view.h>

#include <string>

namespace wave_generator::view::node {

class NodeView {
  public:
    explicit NodeView(ImVec2 position = {});
    void Render(ImDrawList* draw_list, ImVec2 offset = {});

    auto GetID() -> int;

  protected:
    virtual auto GetName() -> const std::string& = 0;

  private:
    static auto GenerateId() -> int;

    static const ImRect kPadding;
    static int id_counter_;
    static const ImColor kBackgroundColor;
    static const ImColor kBorderColor;

    ImVec2 position_{};
    ImVec2 size_{};
    int id_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEW_H
