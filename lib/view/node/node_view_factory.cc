#include "node_view_factory.h"

namespace wave_generator::view::node {

NodeViewFactoryStorage::NodeViewFactoryEntry::NodeViewFactoryEntry(std::string name,
                                                                   Constructor constructor)
    : name_{std::move(name)}, constructor_{std::move(constructor)} {}

const std::string& NodeViewFactoryStorage::NodeViewFactoryEntry::GetName() const { return name_; }

auto NodeViewFactoryStorage::NodeViewFactoryEntry::Construct(ImVec2 position) const
    -> std::shared_ptr<NodeView> {
    return std::shared_ptr<NodeView>{constructor_(position)};
}

auto NodeViewFactoryStorage::GetFactories() -> const std::vector<NodeViewFactoryEntry>& {
    return factories_;
}

void NodeViewFactoryStorage::RegisterFactory(
    const std::string& name, const NodeViewFactoryStorage::Constructor& constructor) {
    factories_.emplace_back(name, constructor);
}

}  // namespace wave_generator::view::node
