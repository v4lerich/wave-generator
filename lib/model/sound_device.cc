#include "sound_device.h"

#include <algorithm>
#include <atomic>

namespace wave_generator::model {

SoundDevice::SoundDevice(Config config)
    : config_{config} {
    audio_spec_.freq = config.frequency;
    audio_spec_.callback = PlayCallback;
    audio_spec_.channels = 1;
    audio_spec_.userdata = this;
    audio_spec_.samples = config.samples;
    audio_spec_.format = AUDIO_F32;

    if (SDL_OpenAudio(&audio_spec_, nullptr) < 0) {
        exit(-1);
    }
}

SoundDevice::~SoundDevice() {
    SDL_CloseAudio();
}

auto SoundDevice::HasGenerator() const -> bool { return bool(std::atomic_load(&generator_)); }

void SoundDevice::SetGenerator(SignalGeneratorPtr generator) {
    std::atomic_store(&generator_, std::move(generator));
}

auto SoundDevice::IsPlaying() const -> bool { return is_playing_; }

void SoundDevice::Play() {
    if (generator_) {
        generator_->Reset();
    }
    is_playing_ = true;
    SDL_PauseAudio(SDL_FALSE);
}

void SoundDevice::Pause() {
    is_playing_ = false;
    SDL_PauseAudio(SDL_TRUE);
}

void SoundDevice::FillStream(SignalGeneratorPtr generator, uint8_t* stream, int length) {
    while (length > 0) {
        auto& sample = sample_;
        auto sample_size = std::min(int(kSampleSize), length);
        auto delta = length / double(config_.frequency);

        std::generate(
            std::begin(sample),
            std::next(std::begin(sample), sample_size),
            [&] () { return generator->SampleAfter(delta); });

        auto sample_byte_size = SDL_AUDIO_BITSIZE(audio_spec_.format) / 8 * sample_size;
        SDL_memcpy(stream, sample_.data(), sample_size);

        length -= sample_size;
    }
}

void SoundDevice::PlayCallback(void* device_pointer, uint8_t* stream, int length) {
    auto device = static_cast<SoundDevice*>(device_pointer);
    auto generator = std::atomic_load(&device->generator_);

    if (!generator) {
        device->Pause();
    } else {
        device->FillStream(generator, stream, length);
    }
}

}