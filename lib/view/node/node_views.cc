#include "node_views.h"

#include <constant_generator.h>
#include <pulse_generator.h>
#include <sawtooth_generator.h>
#include <sine_generator.h>
#include <sound_device.h>
#include <triangular_generator.h>
#include <white_noise_generator.h>

#include <algorithm>
#include <utility>

namespace wave_generator::view::node {

SignalGeneratorNodeView::SignalGeneratorNodeView(std::string name, ImVec2 position)
    : NodeView{std::move(name), position} {}

SignalSinkNodeView::SignalSinkNodeView(model::SoundDevicePtr sound_device, ImVec2 position)
    : NodeView{"Sink", position},
      mode_input_{this, "Mode", {"Same", "Channels"}},
      same_input_{this, "Input"},
      sound_device_{std::move(sound_device)} {}

auto SignalSinkNodeView::GetInputViews() -> std::list<NodeInputView *> {
    std::list<NodeInputView *> inputs{&mode_input_};
    if (mode_input_.GetCase() == "Same") {
        inputs.push_back(&same_input_);
    } else {
        for (auto &channel_input : channel_inputs_) {
            inputs.push_back(&channel_input);
        }
    }
    return inputs;
}

auto SignalSinkNodeView::CreateGenerators() const -> std::vector<SignalGeneratorPtr> {
    std::vector<SignalGeneratorPtr> generators;
    if (mode_input_.GetCase() == "Same") {
        auto connected_node = same_input_.GetConnectedSignalNode();
        for (auto &channel_input : channel_inputs_) {
            auto generator =
                (connected_node == nullptr) ? nullptr : connected_node->CreateGenerator();
            generators.push_back(std::move(generator));
        }
    } else {
        std::transform(std::begin(channel_inputs_), std::end(channel_inputs_),
                       std::back_inserter(generators), [&](const auto &channel_input) {
                           auto connected_node = channel_input.GetConnectedSignalNode();
                           return (connected_node == nullptr) ? nullptr
                                                              : connected_node->CreateGenerator();
                       });
    }
    return generators;
}

auto SignalSinkNodeView::IsDeletable() const -> bool { return false; }

void SignalSinkNodeView::SetChannels(size_t channels_count) {
    if (channel_inputs_.size() == channels_count) return;

    this->Disconnect();
    channel_inputs_.clear();
    for (size_t i = 0; i < channels_count; i++) {
        channel_inputs_.emplace_back(this, "Input " + std::to_string(i));
    }
}

auto SignalSinkNodeView::GetChannelsCount() -> size_t { return channel_inputs_.size(); }

void SignalSinkNodeView::EndRender() {
    if (mode_input_.HasChanged()) {
        Disconnect();
    }
}

void SignalSinkNodeView::BeginRender() {
    SetChannels(sound_device_->GetConfig().generator_config.channels);
}

AmplitudeFrequencyGeneratorNodeView::AmplitudeFrequencyGeneratorNodeView(std::string name,
                                                                         ImVec2 position)
    : SignalGeneratorNodeView{std::move(name), position},
      amplitude_input_port_{this, "Amplitude"},
      frequency_input_port_{this, "Frequency"},
      base_amplitude_input_{this, "Base amplitude", {0.0, 1.0}, 0.5},
      base_frequency_input_{this, "Base frequency", {0.1, 22000}, 1000, FloatInputView::Type::Logarithmic},
      output_node_{this} {}

auto AmplitudeFrequencyGeneratorNodeView::GenerateAmplitudeSignal() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    auto amplitude_node = amplitude_input_port_.GetConnectedSignalNode();
    return (amplitude_node == nullptr) ? nullptr : amplitude_node->CreateGenerator();
}

auto AmplitudeFrequencyGeneratorNodeView::GenerateFrequencySignal() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    auto frequency_node = frequency_input_port_.GetConnectedSignalNode();
    return (frequency_node == nullptr) ? nullptr : frequency_node->CreateGenerator();
}

auto AmplitudeFrequencyGeneratorNodeView::GetBaseAmplitude() const -> float {
    return base_amplitude_input_.GetValue();
}

auto AmplitudeFrequencyGeneratorNodeView::GetBaseFrequency() const -> float {
    return base_frequency_input_.GetValue();
}

auto AmplitudeFrequencyGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    return {
        &base_amplitude_input_,
        &amplitude_input_port_,
        &base_frequency_input_,
        &frequency_input_port_,
    };
}

auto AmplitudeFrequencyGeneratorNodeView::GetOutputViews() -> std::list<NodeOutputView *> {
    return {&output_node_};
}

ConstantGeneratorNodeView::ConstantGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Constant", position},
      constant_input_{this, "Value"},
      output_node_{this} {}

auto ConstantGeneratorNodeView::GetOutputViews() -> std::list<NodeOutputView *> {
    return {&output_node_};
}

auto ConstantGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    return {&constant_input_};
}

auto ConstantGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::ConstantGenerator>(constant_input_.GetValue());
}

SineGeneratorNodeView::SineGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Sine", position} {}

auto SineGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::SineGenerator>(GetBaseAmplitude(), GetBaseFrequency(),
                                                        GenerateAmplitudeSignal(),
                                                        GenerateFrequencySignal());
}

PulseGeneratorNodeView::PulseGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Pulse", position},
      duty_cycle_input_{this, "Duty cycle", {0.0, 1.0}, 0.5} {}

auto PulseGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::PulseGenerator>(GetDutyCycle(),
                                                        GetBaseAmplitude(), GetBaseFrequency(),
                                                        GenerateAmplitudeSignal(),
                                                        GenerateFrequencySignal());
}

auto PulseGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    auto inputs = AmplitudeFrequencyGeneratorNodeView::GetInputViews();
    inputs.push_front(&duty_cycle_input_);
    return inputs;
}

auto PulseGeneratorNodeView::GetDutyCycle() const -> float { return duty_cycle_input_.GetValue(); }

SawtoothGeneratorNodeView::SawtoothGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Sawtooth", position} {}

auto SawtoothGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::SawtoothGenerator>(GetBaseAmplitude(), GetBaseFrequency(),
                                                            GenerateAmplitudeSignal(),
                                                            GenerateFrequencySignal());
}

TriangularGeneratorNodeView::TriangularGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Triangular", position} {}

auto TriangularGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::TriangularGenerator>(GetBaseAmplitude(), GetBaseFrequency(), GenerateAmplitudeSignal(),
        GenerateFrequencySignal());
}

WhiteNoiseGeneratorNodeView::WhiteNoiseGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"White noise", position},
      base_amplitude_input_{this, "Base amplitude"},
      amplitude_input_node_{this, "Amplitude"},
      output_node_{this} {}

auto WhiteNoiseGeneratorNodeView::GetOutputViews() -> std::list<NodeOutputView *> {
    return {&output_node_};
}

auto WhiteNoiseGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    return {&base_amplitude_input_, &amplitude_input_node_};
}

auto WhiteNoiseGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::WhiteNoiseGenerator>(
        GetBaseAmplitude(), GenerateAmplitude()
        );
}

auto WhiteNoiseGeneratorNodeView::GetBaseAmplitude() const -> float {
    return base_amplitude_input_.GetValue();
}

auto WhiteNoiseGeneratorNodeView::GenerateAmplitude() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    auto connected_node = amplitude_input_node_.GetConnectedSignalNode();
    return connected_node != nullptr ? connected_node->CreateGenerator() : nullptr;
}

}  // namespace wave_generator::view::node
