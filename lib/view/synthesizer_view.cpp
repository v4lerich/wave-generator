#include "synthesizer_view.h"

#include <glad/gl.h>

namespace wave_generator::view {

SynthesizerView::SynthesizerView() = default;

void SynthesizerView::Render() {
    BeginDockingSpace();

    RenderMenuBar();

    EndDockingSpace();
}

void SynthesizerView::BeginDockingSpace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    bool show_dock_window = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &show_dock_window, window_flags);
    ImGui::PopStyleVar();

    const ImGuiDockNodeFlags docking_flags =
        ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockspace_id = ImGui::GetID("main_dockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0, 0.0), docking_flags);
}

void SynthesizerView::EndDockingSpace() {
    ImGui::End();
    ImGui::PopStyleVar();
}

void SynthesizerView::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", nullptr, false))
                SetWantClose();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

}  // namespace wave_generator::view
