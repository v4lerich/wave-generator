#include "application.h"

#include <SDL.h>
#include <glad/gl.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "application_config.h"
#include "glad2_loader.h"

namespace wave_generator {

Application::Application(int /*argc*/, char** /*argv*/) {}

auto Application::Init() -> ReturnCode {
    ReturnCode return_code = 0;
    spdlog::set_level(config::kLogLevel);

    return_code = InitSdl();
    if (return_code != 0) return return_code;

    return_code = CreateMainWindow();
    if (return_code != 0) return -1;

    return_code = InitImGui();
    if (return_code != 0) return -1;

    return 0;
}

auto Application::InitSdl() -> ReturnCode {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        spdlog::critical("SDL Init error: {}", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                        config::kOpenGlVersion.major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                        config::kOpenGlVersion.minor);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    return 0;
}

auto Application::Run() -> ReturnCode {
    SDL_Event sdl_event;
    while (!done_) {
        while (SDL_PollEvent(&sdl_event) == 1) {
            ProcessSdlEvent(sdl_event);
        }
        RenderView();
        SDL_GL_SwapWindow(main_window_);
    }

    return Finalize();
}

auto Application::CreateMainWindow() -> ReturnCode {
    auto window_flags = SDL_WindowFlags(
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    main_window_ = SDL_CreateWindow(
        "Wave generator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config::kWindowWidth, config::kWindowHeight, window_flags);

    gl_context_ = SDL_GL_CreateContext(main_window_);
    SDL_GL_MakeCurrent(main_window_, gl_context_);
    SDL_GL_SetSwapInterval(static_cast<int>(config::kVsyncEnable));

    if (gl_loader::LoadGl()) {
        spdlog::critical("GL Context not loaded");
        return -1;
    }
    return 0;
}

auto Application::InitImGui() -> ReturnCode {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imgui_io_ = &ImGui::GetIO();
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (config::kImGuiTheme == config::ApplicationTheme::kDark) {
        ImGui::StyleColorsDark();
    } else if (config::kImGuiTheme == config::ApplicationTheme::kClassic) {
        ImGui::StyleColorsClassic();
    } else if (config::kImGuiTheme == config::ApplicationTheme::kLight) {
        ImGui::StyleColorsLight();
    }

    ImGui_ImplSDL2_InitForOpenGL(main_window_, gl_context_);
    ImGui_ImplOpenGL3_Init(config::kGlslVersion);
    return 0;
}

void Application::ProcessSdlEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (event.type == SDL_QUIT) {
        done_ = true;
    }

    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE &&
        event.window.windowID == SDL_GetWindowID(main_window_)) {
        done_ = true;
    }
}

void Application::RenderView() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(main_window_);
    ImGui::NewFrame();

    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window_);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window_);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color_); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (show_another_window_)
    {
        ImGui::Begin("Another Window", &show_another_window_);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window_ = false;
        ImGui::End();
    }

    ImGui::Render();
    glViewport(0, 0, (int)imgui_io_->DisplaySize.x, (int)imgui_io_->DisplaySize.y);
    glClearColor(clear_color_.x, clear_color_.y, clear_color_.z, clear_color_.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto Application::Finalize() -> Application::ReturnCode {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(main_window_);
    SDL_Quit();

    return 0;
}

}  // namespace wave_generator