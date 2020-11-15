#ifndef WAVEGENERATOR_SYNTHESIZER_VIEW_H
#define WAVEGENERATOR_SYNTHESIZER_VIEW_H

#include <imgui.h>

#include <memory>

#include "editor_view.h"
#include "player_view.h"
#include "view.h"

namespace wave_generator::view {

class SynthesizerView : public View {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;

    SynthesizerView();
    void Render() override;

  private:
    void BeginDockingWindow();
    void InitDockingLayout();
    void EndDockingWindow();

    void RenderMenuBar();

    auto CreateGenerators() -> std::vector<SignalGeneratorPtr>;

    std::shared_ptr<model::SoundDevice> sound_device_;
    PlayerView player_view_;
    EditorView editor_view_;
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_SYNTHESIZER_VIEW_H
