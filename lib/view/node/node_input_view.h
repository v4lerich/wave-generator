#ifndef WAVEGENERATOR_NODE_INPUT_VIEW_H
#define WAVEGENERATOR_NODE_INPUT_VIEW_H

#include <imgui.h>
#include <view.h>

namespace wave_generator::view::node {

class NodeInputView : public View {
  public:
    void Render() override;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_INPUT_VIEW_H
