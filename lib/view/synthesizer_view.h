#ifndef WAVEGENERATOR_SYNTHESIZER_VIEW_H
#define WAVEGENERATOR_SYNTHESIZER_VIEW_H

#include <memory>
#include <imgui.h>

#include "view.h"
#include "editor_view.h"

namespace wave_generator::view {

class SynthesizerView : public View {
  public:
    SynthesizerView();
    void Render() override;

  private:
    void BeginDockingWindow();
    void InitDockingLayout();
    void EndDockingWindow();

    void RenderMenuBar();

    static const std::string kNodesWindowName;
    static const std::string kDockspaceName;

    std::unique_ptr<EditorView> editor_view_;
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_SYNTHESIZER_VIEW_H
