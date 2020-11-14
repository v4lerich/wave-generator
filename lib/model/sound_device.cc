#include "sound_device.h"

#include <algorithm>
#include <atomic>
#include <utility>

namespace wave_generator::model {

SoundDevice::SoundDevice(Config config)
    : config_{config}, cacher_{{
        .samples_chunk_size = config.samples_chunk_size,
        .cache_samples_chunks = config.cache_samples_chunks,
        .generator_config = {
            .frequency = config.frequency,
            .buffer_size = config.samples,
        }
    }} {

    audio_spec_.freq = config.frequency;
    audio_spec_.callback = GenerateSamples;
    audio_spec_.channels = 1;
    audio_spec_.userdata = this;
    audio_spec_.samples = config.samples;
    audio_spec_.format = AUDIO_F32;

    if (SDL_OpenAudio(&audio_spec_, nullptr) < 0) {
        exit(-1);
    }

    Reset();
}

SoundDevice::~SoundDevice() {
    SDL_CloseAudio();
}

void SoundDevice::SetGenerator(SignalGeneratorPtr generator) {
    Reset();
    cacher_.SetGenerator(std::move(generator));
}

auto SoundDevice::IsPlaying() const -> bool { return is_playing_; }

void SoundDevice::Play() {
    is_playing_ = true;
    cacher_.Start();
    SDL_PauseAudio(SDL_FALSE);
}

void SoundDevice::Pause() {
    is_playing_ = false;
    cacher_.Stop();
    SDL_PauseAudio(SDL_TRUE);
}

void SoundDevice::GenerateSamples(void* device_pointer, uint8_t* stream, int length) {
    auto device = static_cast<SoundDevice *>(device_pointer);
    device->GenerateSamples(stream, length);
}

void SoundDevice::GenerateSamples(uint8_t* stream, size_t length) {
    cacher_.GenerateSamples(reinterpret_cast<float*>(stream), length);
}

void SoundDevice::Reset() {
    buffer_.resize(config_.buffer_size);
}

}