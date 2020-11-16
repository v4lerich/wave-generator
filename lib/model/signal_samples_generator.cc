#include "signal_samples_generator.h"

#include <algorithm>
#include <utility>

namespace wave_generator::model {

SignalSamplesGenerator::SignalSamplesGenerator(Config config)
    : generators_(config.channels), config_{config}
{}

void SignalSamplesGenerator::GenerateSamples(float* buffer, size_t samples_count) {
    std::unique_lock<std::mutex> lock{mutex_};

    auto sample_step = 1.0 / config_.frequency;

    for (auto i = 0; i < (samples_count / config_.channels); i++) {
        GenerateSample(buffer, sample_step);
        std::advance(buffer, config_.channels);
    }
}

void SignalSamplesGenerator::GenerateSample(float* buffer,
                                            double sample_step) const {
    for (auto& generator : generators_) {
        auto loaded_generator = std::atomic_load(&generator);
        *buffer = loaded_generator ? float(loaded_generator->SampleAfter(sample_step)) : 0;
        std::advance(buffer, 1);
    }
}

void SignalSamplesGenerator::SetGenerator(size_t channel, SignalGeneratorPtr generator) {
    auto& target_generator = generators_[channel];
    std::atomic_store(&target_generator, std::move(generator));
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
