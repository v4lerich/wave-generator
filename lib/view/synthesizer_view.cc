#include "synthesizer_view.h"

#include <imgui_internal.h>

namespace wave_generator::view {

const std::string SynthesizerView::kNodesWindowName = "nodes";
const std::string SynthesizerView::kDockspaceName = "main_dockspace";

SynthesizerView::SynthesizerView() : editor_view_{new EditorView{}} {}

void SynthesizerView::Render() {
    BeginDockingWindow();
    RenderMenuBar();

    editor_view_->Render();

    auto window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin(kNodesWindowName.c_str(), nullptr, window_flags);
    ImGui::End();

    EndDockingWindow();
}

void SynthesizerView::BeginDockingWindow() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    bool show_dock_window = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &show_dock_window, window_flags);
    ImGui::PopStyleVar();

    const ImGuiDockNodeFlags docking_flags =
        ImGuiDockNodeFlags_PassthruCentralNode;
    InitDockingLayout();
    ImGui::DockSpace(ImGui::GetID(kDockspaceName.c_str()), {0, 0});
}

void SynthesizerView::InitDockingLayout() {
    if (!ImGui::DockBuilderGetNode(ImGui::GetID(kDockspaceName.c_str()))) {
        ImGuiID dockspace_id = ImGui::GetID(kDockspaceName.c_str());
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id,
                                      ImGui::GetWindowViewport()->Size);

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(
            dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow(editor_view_->WindowName().c_str(),
                                     dock_main_id);
        ImGui::DockBuilderDockWindow(kNodesWindowName.c_str(), dock_left_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }
}

void SynthesizerView::EndDockingWindow() {
    ImGui::End();
    ImGui::PopStyleVar();
}

void SynthesizerView::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", nullptr, false)) SetWantClose();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

}  // namespace wave_generator::view
