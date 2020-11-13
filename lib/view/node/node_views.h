#ifndef WAVEGENERATOR_NODE_VIEWS_H
#define WAVEGENERATOR_NODE_VIEWS_H

#include <signal_generator.h>

#include <memory>

#include "node_port_views.h"
#include "node_view.h"

namespace wave_generator::view::node {

class SignalGeneratorNodeView : public NodeView {
  public:
    explicit SignalGeneratorNodeView(std::string name, ImVec2 position = {});
    virtual auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> = 0;
};

class SignalSinkNodeView final : public SignalGeneratorNodeView {
  public:
    explicit SignalSinkNodeView(ImVec2 position = {});
    auto CreateGenerator() const -> std::unique_ptr<synthesizer::SignalGenerator> override;
    auto IsDeletable() const -> bool override;

  protected:
    auto GetInputViews() -> std::list<NodeInputView *> override;

  private:
    SignalPortInputView input_node_;
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
    FloatInputView base_amplitude_input_;
    SignalPortInputView amplitude_input_node_;
    SignalPortOutputView output_node_;
};

}  // namespace wave_generator::view::node

#endif  // WAVEGENERATOR_NODE_VIEWS_H
