#ifndef WAVEGENERATOR_APPLICATION_H
#define WAVEGENERATOR_APPLICATION_H

#include <SDL.h>
#include <imgui/imgui.h>
#include <view.h>

#include <memory>
#include <vector>

namespace wave_generator {

using view::View;

class Application {
  public:
    using ReturnCode = int;

    Application(int argc, char **argv);
    void AddView(std::unique_ptr<View> view);
    auto Init() -> ReturnCode;
    auto Run()  -> ReturnCode;

  private:
    static auto InitSdl() -> ReturnCode;
    auto CreateMainWindow() -> ReturnCode;
    auto InitImGui() -> ReturnCode;

    auto Finalize() -> ReturnCode;

    void ProcessSdlEvent(const SDL_Event& event);
    void RenderView();

    SDL_Window* main_window_{};
    SDL_GLContext gl_context_{};
    ImGuiIO* imgui_io_{};
    bool done_{};

    std::vector<std::unique_ptr<View>> views_{};
};

}

#endif  // WAVEGENERATOR_APPLICATION_H
