#ifndef WAVEGENERATOR_NODE_INPUT_VIEWS_H
#define WAVEGENERATOR_NODE_INPUT_VIEWS_H

#include "node_input_view.h"

namespace wave_generator::view::node {

class SignalNodeInputView final : public NodeInputView {
  public:
    explicit SignalNodeInputView(std::string name);

    void RenderItem(ImDrawList *draw_list) const override;
};

}

#endif  // WAVEGENERATOR_NODE_INPUT_VIEWS_H
