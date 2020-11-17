#include "node_views.h"

#include <constant_generator.h>
#include <mixer_generator.h>
#include <pulse_generator.h>
#include <sawtooth_generator.h>
#include <sine_generator.h>
#include <sound_device.h>
#include <triangular_generator.h>
#include <white_noise_generator.h>

#include <algorithm>
#include <utility>

namespace wave_generator::view::node {

constexpr float kEpsilon = 1e-5;

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
        for (auto &channel_input : channel_inputs_) {
            auto generator = same_input_.CreateConnectedGenerator();
            generators.push_back(std::move(generator));
        }
    } else {
        std::transform(
            std::begin(channel_inputs_), std::end(channel_inputs_), std::back_inserter(generators),
            [&](const auto &channel_input) { return channel_input.CreateConnectedGenerator(); });
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
      base_frequency_input_{
          this, "Base frequency", {0.1, 22000}, 1000, FloatInputView::Type::Logarithmic},
      output_node_{this} {}

auto AmplitudeFrequencyGeneratorNodeView::GenerateAmplitudeSignal() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return amplitude_input_port_.CreateConnectedGenerator();
}

auto AmplitudeFrequencyGeneratorNodeView::GenerateFrequencySignal() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return frequency_input_port_.CreateConnectedGenerator();
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
    return std::make_unique<synthesizer::PulseGenerator>(
        GetDutyCycle(), GetBaseAmplitude(), GetBaseFrequency(), GenerateAmplitudeSignal(),
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
    return std::make_unique<synthesizer::TriangularGenerator>(
        GetBaseAmplitude(), GetBaseFrequency(), GenerateAmplitudeSignal(),
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
    return std::make_unique<synthesizer::WhiteNoiseGenerator>(GetBaseAmplitude(),
                                                              GenerateAmplitude());
}

auto WhiteNoiseGeneratorNodeView::GetBaseAmplitude() const -> float {
    return base_amplitude_input_.GetValue();
}

auto WhiteNoiseGeneratorNodeView::GenerateAmplitude() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    return amplitude_input_node_.CreateConnectedGenerator();
}

MixerGeneratorNodeView::MixerGeneratorNodeView(ImVec2 position)
    : SignalGeneratorNodeView{"Mixer", position},
      input_counts_{this, "Input Counts", {1, 100}, 2},
      output_node_{this, "Output"} {
    Refresh();
}

auto MixerGeneratorNodeView::GetInputViews() -> std::list<NodeInputView *> {
    std::list<NodeInputView *> inputs{&input_counts_};
    for (auto &input : inputs_) {
        inputs.push_back(&input.coefficient_node);
        inputs.push_back(&input.input_node);
    }
    return inputs;
}

auto MixerGeneratorNodeView::GetOutputViews() -> std::list<NodeOutputView *> {
    return {&output_node_};
}

auto MixerGeneratorNodeView::CreateGenerator() const
    -> std::unique_ptr<synthesizer::SignalGenerator> {
    std::vector<synthesizer::MixerGenerator::MixerInputSignal> mixer_inputs;
    for (auto &input : inputs_) {
        mixer_inputs.push_back({
            .coefficient = input.coefficient_node.GetValue(),
            .generator = input.input_node.CreateConnectedGenerator(),
        });
    }
    return std::make_unique<synthesizer::MixerGenerator>(std::move(mixer_inputs));
}

void MixerGeneratorNodeView::Refresh() {
    while (inputs_.size() < GetInputCounts()) {
        int id = inputs_.size() + 1;
        inputs_.push_back({
            .coefficient_node =
                FloatInputView{this, "Coefficient " + std::to_string(id), {0.0, 1.0}, 0.0},
            .input_node = SignalPortInputView{this, "Input " + std::to_string(id)},
        });
    }

    while (inputs_.size() > GetInputCounts()) {
        inputs_.back().input_node.Disconnect();
        inputs_.pop_back();
    }
}

auto MixerGeneratorNodeView::GetInputCounts() -> int { return input_counts_.GetValue(); }

void MixerGeneratorNodeView::EndRender() { Refresh(); }

void MixerGeneratorNodeView::BeginRender() {
    double coefficient_sum = 0;
    for (auto &input : inputs_) {
        coefficient_sum += input.coefficient_node.GetValue();
    }
    if (std::abs(coefficient_sum) < kEpsilon) {
        for (auto &input : inputs_) {
            input.coefficient_node.SetValue(1.0 / inputs_.size());
        }
    } else {
        for (auto &input : inputs_) {
            input.coefficient_node.SetValue(input.coefficient_node.GetValue() / coefficient_sum);
        }
    }
}

}  // namespace wave_generator::view::node
