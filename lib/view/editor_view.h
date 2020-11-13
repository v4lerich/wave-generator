#ifndef WAVEGENERATOR_EDITOR_VIEW_H
#define WAVEGENERATOR_EDITOR_VIEW_H

#include <node/node_view_factory.h>
#include <node/node_views.h>

#include <list>
#include <memory>
#include <string>

#include "node/node_view.h"
#include "view.h"

namespace wave_generator::view {

class EditorView : public View {
  public:
    EditorView();

    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();
    void RenderPopup(ImVec2 offset);

    node::NodeViewFactoryStorage factory_storage_{};
    std::list<std::shared_ptr<node::NodeView>> nodes_{};
    std::shared_ptr<node::NodeView> sink_;

    ImVec2 scrolling_offset_{};
};

};  // namespace wave_generator::view

#endif  // WAVEGENERATOR_EDITOR_VIEW_H
