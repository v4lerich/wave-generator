#ifndef WAVEGENERATOR_NODE_INPUT_VIEW_H
#define WAVEGENERATOR_NODE_INPUT_VIEW_H

#include <imgui.h>
#include <view.h>
#include <string>

namespace wave_generator::view::node {

class NodeInputView {
  public:
    explicit NodeInputView(std::string name);
    void Render(ImDrawList *draw_list) const;

  protected:
    virtual void RenderItem(ImDrawList *draw_list) const = 0;
    auto GetName() const -> const std::string&;

  private:
    std::string name_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_INPUT_VIEW_H
