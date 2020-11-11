#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include "../../synthesizer/constant_generator.h"
#include "node_view.h"

#include <memory>

namespace wave_generator::view::node {

class ConstantGeneratorNodeView : public NodeView {
  public:
    explicit ConstantGeneratorNodeView(
        std::shared_ptr<synthesizer::ConstantGenerator> generator);

  protected:
    std::shared_ptr<synthesizer::ConstantGenerator> generator;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
