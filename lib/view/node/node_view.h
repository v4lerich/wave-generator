#ifndef WAVEGENERATOR_NODE_VIEW_H
#define WAVEGENERATOR_NODE_VIEW_H

#include <imgui.h>
#include <view.h>

#include <string>

namespace wave_generator::view::node {

class NodeView : public View {
  public:
    void Render() override;

  protected:
    virtual const std::string& GetName() = 0;

    ImVec2 position{};
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEW_H
