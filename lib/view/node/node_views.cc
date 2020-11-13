#include "node_views.h"

#include <constant_generator.h>
#include <pulse_generator.h>
#include <sawtooth_generator.h>
#include <sine_generator.h>
#include <triangular_generator.h>

#include <utility>

namespace wave_generator::view::node {

SignalGeneratorNodeView::SignalGeneratorNodeView(std::string name, ImVec2 position)
    : NodeView{std::move(name), position} {}

SignalSinkNodeView::SignalSinkNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Sink", position}, input_node_{this, "Input"} {}

std::list<NodeInputView *> SignalSinkNodeView::GetInputViews() { return {&input_node_}; }

auto SignalSinkNodeView::CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> {
    auto connected_node = input_node_.GetConnectedSignalNode();
    return (connected_node == nullptr) ? nullptr : connected_node->CreateGenerator();
}

auto SignalSinkNodeView::IsDeletable() const -> bool { return false; }

AmplitudeFrequencyGeneratorNodeView::AmplitudeFrequencyGeneratorNodeView(std::string name,
                                                                         ImVec2 position)
    : SignalGeneratorNodeView{std::move(name), position},
      amplitude_input_port_{this, "Amplitude"},
      frequency_input_port_{this, "Frequency"},
      base_amplitude_input_{this, "Base amplitude"},
      base_frequency_input_{this, "Base frequency"},
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
    return std::make_unique<synthesizer::SineGenerator>(GetBaseFrequency(), GetBaseAmplitude(),
                                                        GenerateAmplitudeSignal(),
                                                        GenerateFrequencySignal());
}

PulseGeneratorNodeView::PulseGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Pulse", position},
      duty_cycle_input_{this, "Duty cycle", {0.0, 1.0}, 0.5} {}

auto PulseGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::SineGenerator>(GetBaseFrequency(), GetBaseAmplitude(),
                                                        GenerateAmplitudeSignal(),
                                                        GenerateFrequencySignal());
}

auto PulseGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    auto inputs = AmplitudeFrequencyGeneratorNodeView::GetInputViews();
    inputs.push_front(&duty_cycle_input_);
    return inputs;
}

SawtoothGeneratorNodeView::SawtoothGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Sawtooth", position} {}

auto SawtoothGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::SawtoothGenerator>(GetBaseFrequency(), GetBaseAmplitude(),
                                                            GenerateAmplitudeSignal(),
                                                            GenerateFrequencySignal());
}

TriangularGeneratorNodeView::TriangularGeneratorNodeView(ImVec2 position)
    : AmplitudeFrequencyGeneratorNodeView{"Triangular", position} {}

auto TriangularGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return std::make_unique<synthesizer::TriangularGenerator>(
        GetBaseFrequency(), GetBaseAmplitude(), GenerateAmplitudeSignal(),
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
    return std::unique_ptr<synthesizer::SignalGenerator>();
}

}  // namespace wave_generator::view::node
