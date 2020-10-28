#ifndef WAVE_GENERATOR_APPLICATION_H
#define WAVE_GENERATOR_APPLICATION_H

#include <SDL.h>
#include <imgui/imgui.h>

#include <memory>

namespace wave_generator {

class Application {
  public:
    using ReturnCode = int;

    Application(int argc, char **argv);
    auto Init() -> ReturnCode;
    auto Run() -> ReturnCode;

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

    bool show_demo_window_{true};
    bool show_another_window_{true};
    ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

}  // namespace wave_generator

#endif  // WAVE_GENERATOR_APPLICATION_H
