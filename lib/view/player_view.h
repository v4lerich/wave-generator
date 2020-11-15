#ifndef WAVEGENERATOR_PLAYER_VIEW_H
#define WAVEGENERATOR_PLAYER_VIEW_H

#include <sound_device.h>

#include <memory>
#include <functional>
#include <string>

#include "view.h"

namespace wave_generator::view {

class PlayerView : public View {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;
    using GeneratorsBuilderFunc = std::function<std::vector<SignalGeneratorPtr> ()>;

    explicit PlayerView(model::SoundDevicePtr sound_device, GeneratorsBuilderFunc generators_builder);

    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();

    GeneratorsBuilderFunc generators_builder_;
    std::shared_ptr<model::SoundDevice> sound_device_;
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_PLAYER_VIEW_H
