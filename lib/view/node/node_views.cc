#include "node_views.h"

#include <utility>

namespace wave_generator::view::node {

ConstantGeneratorNodeView::ConstantGeneratorNodeView(
    std::shared_ptr<synthesizer::ConstantGenerator> generator)
    : generator(std::move(generator)) {}

}  // namespace wave_generator::view::node
