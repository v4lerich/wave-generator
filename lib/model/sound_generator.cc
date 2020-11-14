#include "sound_generator.h"

#include <utility>
#include <algorithm>

namespace wave_generator::model {

SoundGenerator::SoundGenerator(Config config, SignalGeneratorPtr generator)
    : generator_{std::move(generator)}, config_{config}, samples_buffer_(config_.buffer_size)
{}

auto SoundGenerator::GetFormat() const -> SDL_AudioFormat { return AUDIO_F32; }

auto SoundGenerator::GenerateSamples(uint8_t* buffer, size_t samples_count) const -> bool {
    std::unique_lock<std::mutex> lock{generator_mutex_};

    auto generator = std::atomic_load(&generator_);

    if (generator) {
        GenerateSamples(generator, buffer, samples_count);
    }

    return bool(generator);
}

void SoundGenerator::GenerateSamples(SignalGeneratorPtr generator, uint8_t* buffer,
                                     size_t samples_count) const {
    while (samples_count > 0) {
        auto batch_length = std::min(config_.buffer_size, samples_count);
        auto sample_step = 1.0 / config_.frequency;

        std::generate_n(std::begin(samples_buffer_), batch_length,
                        [&] () { return float(generator->SampleAfter(sample_step)); });

        auto batch_bytes_count = sizeof(decltype(samples_buffer_)::value_type) * samples_count;
        memcpy(buffer, samples_buffer_.data(), batch_bytes_count);

        samples_count -= batch_length;
        buffer += batch_bytes_count;
    }
}

void SoundGenerator::SetGenerator(SignalGeneratorPtr generator) {
    std::atomic_store(&generator_, std::move(generator));
}

auto SoundGenerator::CanGenerate() const -> bool {
    auto generator = std::atomic_load(&generator_);
    return bool(generator);
}

void SoundGenerator::Reset() {
    std::unique_lock<std::mutex> lock{generator_mutex_};

    if (generator_) {
        generator_->Reset();
    }
}

}
