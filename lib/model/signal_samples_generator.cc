#include "signal_samples_generator.h"

#include <algorithm>
#include <utility>

namespace wave_generator::model {

SignalSamplesGenerator::SignalSamplesGenerator(Config config)
    : generators_(config.channels), config_{config}
{}

auto SignalSamplesGenerator::GenerateSamples(float* buffer, size_t samples_count) -> bool {
    std::unique_lock<std::mutex> lock{mutex_};

    auto sample_step = 1.0 / config_.frequency;

    for (auto i = 0; i < (samples_count / config_.channels); i++) {
        if (!GenerateSample(buffer, sample_step)) { return false; }
        std::advance(buffer, config_.channels);
    }
    return true;
}

auto SignalSamplesGenerator::GenerateSample(float* buffer,
                                            double sample_step) const -> bool {
    for (auto& generator : generators_) {
        if (generator) {
            *buffer = float(generator->SampleAfter(sample_step));
        } else {
            return false;
        }
        std::advance(buffer, 1);
    }
    return true;
}

void SignalSamplesGenerator::SetGenerator(size_t channel, SignalGeneratorPtr generator) {
    std::atomic_store(&generators_[channel], std::move(generator));
}

auto SignalSamplesGenerator::CanGenerate() const -> bool {
    for (size_t i = 0; i < config_.channels; i++) {
        auto generator = std::atomic_load(&generators_[i]);
        if (!generator) {
            return false;
        }
    }
    return true;
}

void SignalSamplesGenerator::Reset() {
    std::unique_lock<std::mutex> lock{mutex_};

    for (auto& generator : generators_) {
        if (generator) {
            generator->Reset();
        }
    }
}

void SignalSamplesGenerator::UpdateConfig(Config config) {
    std::unique_lock<std::mutex> lock{mutex_};

    config_ = config;
}

}
