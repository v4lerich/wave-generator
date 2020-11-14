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
    while (samples_count > 0) {
        auto batch_length = std::min(config_.buffer_size, samples_count);
        auto sample_step = 1.0 / config_.frequency;

        std::generate_n(std::begin(samples_buffer_), batch_length,
                        [&] () { return float(generator->SampleAfter(sample_step)); });

        memcpy(buffer, samples_buffer_.data(), batch_length);

        samples_count -= batch_length;
        buffer += batch_length;
    }
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
