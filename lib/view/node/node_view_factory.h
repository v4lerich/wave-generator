#ifndef WAVEGENERATOR_NODE_VIEW_FACTORY_H
#define WAVEGENERATOR_NODE_VIEW_FACTORY_H

#include <functional>
#include <vector>

#include "node_view.h"

namespace wave_generator::view::node {

class NodeViewFactoryStorage {
  public:
    using Constructor = std::function<std::shared_ptr<NodeView>(ImVec2)>;

    class NodeViewFactoryEntry {
      public:
        NodeViewFactoryEntry(std::string name, Constructor constructor);
        auto GetName() const -> const std::string&;
        auto Construct(ImVec2 position) const -> std::shared_ptr<NodeView>;

      private:
        std::string name_;
        Constructor constructor_;
    };

    void RegisterFactory(const std::string& name, const Constructor& constructor);
    auto GetFactories() -> const std::vector<NodeViewFactoryEntry>&;

  private:
    std::vector<NodeViewFactoryEntry> factories_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEW_FACTORY_H
