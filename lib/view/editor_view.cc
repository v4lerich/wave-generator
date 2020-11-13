#include "editor_view.h"

#include <imgui.h>
#include <node/node_views.h>

#include <algorithm>

namespace wave_generator::view {

const std::string kWindowName = "editor";
const std::string kContextPopupName = "context_popup";

EditorView::EditorView() {
    sink_ = std::make_shared<node::SignalSinkNodeView>();
    sink_->Move({300, 300});
    nodes_.push_back(sink_);

    factory_storage_.RegisterFactory("Constant", [](ImVec2 position) {
        return std::make_shared<node::ConstantGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("Pulse", [](ImVec2 position) {
        return std::make_shared<node::PulseGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("Sine", [](ImVec2 position) {
        return std::make_shared<node::SineGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("Triangular", [](ImVec2 position) {
        return std::make_shared<node::TriangularGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("Sawtooth", [](ImVec2 position) {
        return std::make_shared<node::SawtoothGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("Pulse", [](ImVec2 position) {
        return std::make_shared<node::PulseGeneratorNodeView>(position);
    });
    factory_storage_.RegisterFactory("White noise", [](ImVec2 position) {
        return std::make_shared<node::WhiteNoiseGeneratorNodeView>(position);
    });
}

void EditorView::Render() { RenderWindow(); }

const std::string& EditorView::WindowName() { return kWindowName; }

void EditorView::RenderWindow() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin(EditorView::WindowName().c_str(), nullptr, window_flags)) {
        return;
    }

    auto offset = ImGui::GetCursorScreenPos() + scrolling_offset_;
    auto draw_list = ImGui::GetWindowDrawList();

    draw_list->ChannelsSplit(2 * nodes_.size());
    int channel_offset = 2 * nodes_.size();
    for (auto it = std::begin(nodes_); it != std::end(nodes_);
         it = std::next(it), channel_offset -= 2) {
        auto node = *it;

        node->SetChannels(channel_offset - 1, channel_offset - 2);
        node->Render(draw_list, offset);

        if (node->IsActive()) {
            auto move_delta = ImGui::GetIO().MouseDelta;
            node->Move(move_delta);

            auto node_it = it;
            std::rotate(std::begin(nodes_), node_it, std::next(node_it));
        }

        if (node->IsContextOpen()) {
            ImGui::OpenPopup(kContextPopupName.c_str());
        }

        if (node->IsConnecting()) {
            auto mouse_position = ImGui::GetIO().MousePos;
            auto output = node->GetConnectingOutput();
            auto found_node = std::find_if(
                std::begin(nodes_), std::end(nodes_),
                [&](const auto& other_node) { return other_node->GetInput(mouse_position); });

            if (found_node != std::end(nodes_)) {
                auto found_input = (*found_node)->GetInput(mouse_position);
                if (found_input->CanConnect(output)) {
                    found_input->Connect(output);
                }
            }
        }
    }
    draw_list->ChannelsMerge();

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) &&
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
        !ImGui::IsAnyItemHovered()) {
        ImGui::OpenPopup(kContextPopupName.c_str());
    }
    RenderPopup({});

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        scrolling_offset_ += ImGui::GetIO().MouseDelta;
        scrolling_offset_.x = std::max(scrolling_offset_.x, 0.0F);
        scrolling_offset_.y = std::max(scrolling_offset_.y, 0.0F);
    }
    ImGui::End();
}

void EditorView::RenderPopup(ImVec2 offset) {
    if (!ImGui::BeginPopup(kContextPopupName.c_str())) {
        return;
    }

    ImVec2 position = ImGui::GetMousePosOnOpeningCurrentPopup() + offset;
    auto found_node = std::find_if(std::begin(nodes_), std::end(nodes_), [&](const auto& node) {
        return node->GetOuterRect().Contains(position);
    });

    if (found_node != std::end(nodes_)) {
        if ((*found_node)->IsDeletable() && ImGui::MenuItem("Delete")) {
            (*found_node)->Disconnect();
            nodes_.erase(found_node);
            ImGui::CloseCurrentPopup();
        }
    } else {
        if (ImGui::BeginMenu("Add")) {
            for (const auto& factory : factory_storage_.GetFactories()) {
                if (ImGui::MenuItem(factory.GetName().c_str())) {
                    auto node = factory.Construct(position);
                    nodes_.push_front(node);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndPopup();
}

}  // namespace wave_generator::view
