#ifndef WAVEGENERATOR_PLAYER_VIEW_H
#define WAVEGENERATOR_PLAYER_VIEW_H

#include "view.h"
#include <string>

namespace wave_generator::view {

class PlayerView : public View {
  public:
    PlayerView();

    auto IsPlaying() -> bool;
    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();

    bool is_playing_{};
};

}

#endif  // WAVEGENERATOR_PLAYER_VIEW_H
