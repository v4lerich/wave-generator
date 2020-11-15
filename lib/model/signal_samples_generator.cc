#include "signal_samples_generator.h"

#include <algorithm>
#include <utility>

namespace wave_generator::model {

SignalSamplesGenerator::SignalSamplesGenerator(Config config, SignalGeneratorPtr generator)
    : generator_{std::move(generator)}, config_{config}, samples_buffer_(config_.buffer_size)
{}

auto SignalSamplesGenerator::GenerateSamples(float* buffer, size_t samples_count) -> bool {
    std::unique_lock<std::mutex> lock{generator_mutex_};

    auto generator = std::atomic_load(&generator_);

    if (generator) {
        GenerateSamples(generator, buffer, samples_count);
    }

    return bool(generator);
}

void SignalSamplesGenerator::GenerateSamples(SignalGeneratorPtr generator, float* buffer,
                                     size_t samples_count) {
    auto sample_step = 1.0 / config_.frequency;
    std::generate_n(buffer, samples_count,
                    [&] () { return float(generator->SampleAfter(sample_step)); });
}

void SignalSamplesGenerator::SetGenerator(SignalGeneratorPtr generator) {
    std::atomic_store(&generator_, std::move(generator));
}

auto SignalSamplesGenerator::CanGenerate() const -> bool {
    auto generator = std::atomic_load(&generator_);
    return bool(generator);
}

void SignalSamplesGenerator::Reset() {
    std::unique_lock<std::mutex> lock{generator_mutex_};

    if (generator_) {
        generator_->Reset();
    }
}

}
