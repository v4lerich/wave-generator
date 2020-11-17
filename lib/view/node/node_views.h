#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <signal_generator.h>
#include <sound_device.h>

#include <memory>

#include "node_port_views.h"
#include "node_view.h"

namespace wave_generator::view::node {

class SignalGeneratorNodeView : public NodeView {
  public:
    explicit SignalGeneratorNodeView(std::string name, ImVec2 position = {});
    virtual auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> = 0;
};

class SignalSinkNodeView final : public NodeView {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;

    explicit SignalSinkNodeView(model::SoundDevicePtr sound_device, ImVec2 position = {});
    auto CreateGenerators() const -> std::vector<SignalGeneratorPtr>;
    auto IsDeletable() const -> bool override;
    auto GetChannelsCount() -> size_t;

  protected:
    void BeginRender() override;
    void EndRender() override;
    auto GetInputViews() -> std::list<NodeInputView *> override;

  private:
    void SetChannels(size_t channels_count);

    model::SoundDevicePtr sound_device_;
    SwitchInputView mode_input_;
    std::vector<SignalPortInputView> channel_inputs_;
    SignalPortInputView same_input_;
};

class AmplitudeFrequencyGeneratorNodeView : public SignalGeneratorNodeView {
  public:
    explicit AmplitudeFrequencyGeneratorNodeView(std::string name, ImVec2 position = {});

  protected:
    auto GenerateAmplitudeSignal() const -> std::unique_ptr<synthesizer::SignalGenerator>;
    auto GenerateFrequencySignal() const -> std::unique_ptr<synthesizer::SignalGenerator>;
    auto GetBaseAmplitude() const -> float;
    auto GetBaseFrequency() const -> float;

    virtual auto GetInputViews() -> std::list<NodeInputView *> override;
    virtual auto GetOutputViews() -> std::list<NodeOutputView *> override;

  private:
    SignalPortInputView frequency_input_port_;
    SignalPortInputView amplitude_input_port_;
    FloatInputView base_frequency_input_;
    FloatInputView base_amplitude_input_;
    SignalPortOutputView output_node_;
};

class ConstantGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit ConstantGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetOutputViews() -> std::list<NodeOutputView *> override;
    auto GetInputViews() -> std::list<NodeInputView *> override;
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    FloatInputView constant_input_;
    SignalPortOutputView output_node_;
};

class SineGeneratorNodeView final : public AmplitudeFrequencyGeneratorNodeView {
  public:
    explicit SineGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;
};

class PulseGeneratorNodeView final : public AmplitudeFrequencyGeneratorNodeView {
  public:
    explicit PulseGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;

  protected:
    auto GetInputViews() -> std::list<NodeInputView *> override;

  private:
    auto GetDutyCycle() const -> float;
    FloatInputView duty_cycle_input_;
};

class SawtoothGeneratorNodeView final : public AmplitudeFrequencyGeneratorNodeView {
  public:
    explicit SawtoothGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;
};

class TriangularGeneratorNodeView final : public AmplitudeFrequencyGeneratorNodeView {
  public:
    explicit TriangularGeneratorNodeView(ImVec2 position = {});
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;
};

class WhiteNoiseGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit WhiteNoiseGeneratorNodeView(ImVec2 position = {});

  protected:
    auto GetOutputViews() -> std::list<NodeOutputView *> override;
    auto GetInputViews() -> std::list<NodeInputView *> override;
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    auto GetBaseAmplitude() const -> float;
    auto GenerateAmplitude() const -> std::unique_ptr<synthesizer::SignalGenerator>;

    FloatInputView base_amplitude_input_;
    SignalPortInputView amplitude_input_node_;
    SignalPortOutputView output_node_;
};

class MixerGeneratorNodeView final : public SignalGeneratorNodeView {
  public:
    explicit MixerGeneratorNodeView(ImVec2 position = {});

  protected:
    void EndRender() override;
    auto GetOutputViews() -> std::list<NodeOutputView *> override;
    auto GetInputViews() -> std::list<NodeInputView *> override;
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;

  private:
    auto GetInputCounts() -> int;
    void Refresh();

    struct MixerInput {
        FloatInputView coefficient_node;
        SignalPortInputView input_node;
    };

    IntInputView input_counts_;
    std::vector<MixerInput> inputs_{};
    SignalPortOutputView output_node_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
