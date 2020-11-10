#ifndef WAVEGENERATOR_SYNTHESIZER_VIEW_H
#define WAVEGENERATOR_SYNTHESIZER_VIEW_H

#include <imgui.h>

#include "view.h"

namespace wave_generator::view {

class SynthesizerView : public View {
  public:
    SynthesizerView();
    void Render() override;

  private:
    void BeginDockingSpace();
    void EndDockingSpace();

    void RenderMenuBar();
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_SYNTHESIZER_VIEW_H
