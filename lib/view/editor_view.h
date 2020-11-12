#ifndef WAVEGENERATOR_EDITOR_VIEW_H
#define WAVEGENERATOR_EDITOR_VIEW_H

#include <memory>
#include <string>
#include <vector>

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

    static const std::string kWindowName;

    std::vector<std::shared_ptr<node::NodeView>> nodes_{};
    ImVec2 scrolling_offset_{};
    bool is_open_{true};
};

};  // namespace wave_generator::view

#endif  // WAVEGENERATOR_EDITOR_VIEW_H
