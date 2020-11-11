#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <constant_generator.h>

#include <memory>

#include "node_view.h"

namespace wave_generator::view::node {

class ConstantGeneratorNodeView final : public NodeView {
  public:
    ConstantGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetName() -> const std::string& override;

  private:
    static const std::string kNodeName;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
