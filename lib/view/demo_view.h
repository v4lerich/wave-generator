#ifndef WAVEGENERATOR_DEMO_VIEW_H
#define WAVEGENERATOR_DEMO_VIEW_H

#include <imgui.h>

#include "view.h"

namespace wave_generator::view {

class DemoView : public View {
  public:
    DemoView();
    void Render() override;

  private:
    ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window_{true};
    bool show_another_window_{true};
};

}

#endif  // WAVEGENERATOR_DEMO_VIEW_H
