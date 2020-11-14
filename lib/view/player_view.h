#ifndef WAVEGENERATOR_PLAYER_VIEW_H
#define WAVEGENERATOR_PLAYER_VIEW_H

#include <sound_device.h>

#include <memory>
#include <string>

#include "view.h"

namespace wave_generator::view {

class PlayerView : public View {
  public:
    PlayerView();

    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();

    std::shared_ptr<model::SoundDevice> sound_device_;
};

}

#endif  // WAVEGENERATOR_PLAYER_VIEW_H
