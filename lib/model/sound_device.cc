#include "sound_device.h"

#include <algorithm>
#include <atomic>
#include <utility>

namespace wave_generator::model {

SoundDevice::SoundDevice(Config config)
    : config_{config}, cacher_{config.cacher_config} {

    audio_spec_.freq = config_.cacher_config.generator_config.frequency;
    audio_spec_.callback = GenerateSamples;
    audio_spec_.channels = config_.cacher_config.generator_config.channels;
    audio_spec_.userdata = this;
    audio_spec_.samples = config_.samples;
    audio_spec_.format = AUDIO_F32;

    SDL_AudioSpec obtained_spec;
    if (SDL_OpenAudio(&audio_spec_, &obtained_spec) < 0 || obtained_spec.format != AUDIO_F32) {
        exit(-1);
    }
    audio_spec_ = obtained_spec;

    config_.cacher_config.generator_config.frequency = audio_spec_.freq;
    config_.cacher_config.generator_config.channels = audio_spec_.channels;
    config_.samples = audio_spec_.samples;

    Reset();
}

SoundDevice::~SoundDevice() {
    cacher_.Shutdown();
    SDL_CloseAudio();
}

void SoundDevice::SetGenerator(size_t channel, SignalGeneratorPtr generator) {
    Reset();
    cacher_.SetGenerator(channel, std::move(generator));
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
    cacher_.GenerateSamples(reinterpret_cast<float*>(stream), length / sizeof(float));
}

void SoundDevice::Reset() {
    cacher_.Reset();
}

auto SoundDevice::GetQueueSize() -> size_t { return cacher_.GetQueueSize(); }

void SoundDevice::UpdateConfig(SoundDevice::Config config) {
    SDL_LockAudio();

    config_ = config;
    cacher_.UpdateConfig(config.cacher_config);
    Reset();

    SDL_UnlockAudio();

}

auto SoundDevice::GetConfig() const -> const Config& {
    return config_;
}

}