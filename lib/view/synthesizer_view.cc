#include "synthesizer_view.h"

#include <imgui_internal.h>

namespace wave_generator::view {

static const std::string kDockspaceName = "main_dockspace";
static const model::SoundDevice::Config kSoundDeviceConfig = {
    .generator_config =
        {
            .frequency = 48000,
            .channels = 2,
        },
};

SynthesizerView::SynthesizerView()
    : sound_device_{std::make_shared<model::SoundDevice>(kSoundDeviceConfig)},
    editor_view_{sound_device_},
    player_view_{sound_device_}
{}

void SynthesizerView::Render() {
    BeginDockingWindow();
    RenderMenuBar();

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

    ImGui::SetNextWindowClass(&window_class);
    editor_view_.Render();

    ImGui::SetNextWindowClass(&window_class);
    player_view_.Render();

    if (editor_view_.IsTopologyChanged()) {
        auto generators = CreateGenerators();
        for (size_t i = 0; i < generators.size(); i++) {
            sound_device_->SetGenerator(i, std::move(generators[i]));
        }
    }

    EndDockingWindow();
}

void SynthesizerView::BeginDockingWindow() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();

    InitDockingLayout();
    ImGui::DockSpace(ImGui::GetID(kDockspaceName.c_str()), {0, 0});
}

void SynthesizerView::InitDockingLayout() {
    if (ImGui::DockBuilderGetNode(ImGui::GetID(kDockspaceName.c_str())) == nullptr) {
        ImGuiID dockspace_id = ImGui::GetID(kDockspaceName.c_str());
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowViewport()->Size);

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_up_id =
            ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow(player_view_.WindowName().c_str(), dock_up_id);
        ImGui::DockBuilderDockWindow(editor_view_.WindowName().c_str(), dock_main_id);
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
            if (ImGui::MenuItem("Exit", nullptr, false)) {
                SetWantClose();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

auto SynthesizerView::CreateGenerators() -> std::vector<SignalGeneratorPtr> {
    return editor_view_.CreateGenerators();
}

}  // namespace wave_generator::view
