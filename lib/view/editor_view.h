#ifndef WAVEGENERATOR_EDITOR_VIEW_H
#define WAVEGENERATOR_EDITOR_VIEW_H

#include "view.h"
#include <string>

namespace wave_generator::view {

class EditorView : public View {
  public:
    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();

    static const std::string kWindowName;

    bool is_open_{true};
};

};

#endif  // WAVEGENERATOR_EDITOR_VIEW_H
