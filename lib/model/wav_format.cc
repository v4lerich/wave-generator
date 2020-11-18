#include "wav_format.h"

#include <bits/unique_ptr.h>

#include <sstream>
#include <utility>

namespace wave_generator::model {

WavChunk::WavChunk(std::string chunk_id) : chunk_id_{std::move(chunk_id)} {}

void WavChunk::Encode(std::ostream& os) const { os << chunk_id_; }

template <typename T>
void WavChunk::Encode(std::ostream& os, const T& value) {
    if constexpr (std::is_integral_v<T>) {
        T temp_value = value;
        for (size_t i = 0; i < sizeof(T); i++) {
            char byte = char(temp_value & 0xffU);
            os.write(&byte, 1);
            temp_value >>= 8;
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        auto temp_value = float(value);
        WavChunk::Encode<uint32_t>(os, *reinterpret_cast<uint32_t*>(&temp_value));
    } else if constexpr (std::is_array_v<T>) {
        for (const auto& e : value) {
            WavChunk::Encode(os, e);
        }
    }
}

FmtWavChunk::FmtWavChunk(WavFormat format) : WavChunk{"fmt "}, format_{format} {}

void FmtWavChunk::Encode(std::ostream& os) const {
    WavChunk::Encode(os);

    std::stringstream format_stream;
    WavChunk::Encode<uint16_t>(format_stream, format_.format_tag);
    WavChunk::Encode(format_stream, format_.channels);
    WavChunk::Encode(format_stream, format_.sampling_rate);
    WavChunk::Encode(format_stream, format_.data_rate);
    WavChunk::Encode(format_stream, format_.block_size);
    WavChunk::Encode(format_stream, format_.bits_per_sample);

    if (const auto& extension = format_.extension; extension) {
        std::stringstream extension_stream;
        WavChunk::Encode(extension_stream, extension->valid_bits_per_sample);
        WavChunk::Encode(extension_stream, extension->channel_mask);
        WavChunk::Encode(extension_stream, extension->sub_format);

        WavChunk::Encode<uint16_t>(format_stream, extension_stream.tellp());
        format_stream << extension_stream.rdbuf();
    } else {
        WavChunk::Encode<uint16_t>(format_stream, 0);
    }

    WavChunk::Encode<uint32_t>(os, format_stream.tellp());
    os << format_stream.rdbuf();
}

template <typename T>
DataWavChunk<T>::DataWavChunk(std::vector<WavSignalChannel<T>> channels)
    : WavChunk{"data"}, channels_{std::move(channels)} {}

template <typename T>
void DataWavChunk<T>::Encode(std::ostream& os) const {
    WavChunk::Encode(os);

    std::stringstream data_stream;

    auto channels = channels_;
    size_t samples_count = 0;
    for (const auto& channel : channels) {
        samples_count = std::max(samples_count, channel.size());
    }

    for (size_t sample_index = 0; sample_index < samples_count; sample_index++) {
        for (const auto& channel : channels) {
            auto sample = (sample_index < channel.size()) ? channel[sample_index] : T{};
            WavChunk::Encode(data_stream, sample);
        }
    }

    if ((data_stream.tellp() % 2) != 0) {
        WavChunk::Encode<uint8_t>(data_stream, 0);
    }

    WavChunk::Encode<uint32_t>(os, data_stream.tellp());
    os << data_stream.rdbuf();
}

FactWavChunk::FactWavChunk(size_t sample_length) : WavChunk{"fact"}, sample_length_{sample_length} {}

void FactWavChunk::Encode(std::ostream& os) const {
    WavChunk::Encode(os);
    WavChunk::Encode<uint32_t>(os, 4);
    WavChunk::Encode<uint32_t>(os, sample_length_);
}

WaveWavChunk::WaveWavChunk(std::vector<std::unique_ptr<WavChunk>> chunks)
    : WavChunk{"WAVE"}, chunks_{std::move(chunks)} {}

void WaveWavChunk::Encode(std::ostream& os) const {
    WavChunk::Encode(os);

    std::stringstream chunks_stream;
    for (const auto& chunk : chunks_) {
        chunk->Encode(chunks_stream);
    }

    os << chunks_stream.rdbuf();
}

RiffWavChunk::RiffWavChunk(std::unique_ptr<WaveWavChunk> wave_chunk)
    : WavChunk{"RIFF"}, wave_chunk_{std::move(wave_chunk)} {}

void RiffWavChunk::Encode(std::ostream& os) const {
    WavChunk::Encode(os);

    std::stringstream chunk_stream;
    wave_chunk_->Encode(chunk_stream);

    WavChunk::Encode<uint32_t>(os, chunk_stream.tellp());
    os << chunk_stream.rdbuf();
}

WavFormatEncoder::WavFormatEncoder(WavFormatEncoder::Config config) : config_{config} {}

void WavFormatEncoder::Encode(std::ostream& os, std::vector<WavSignalChannel<float>> channels) {
    WavFormat format = {
        .format_tag = WavFormat::kWaveFormatIeeeFloat,
        .channels = uint16_t(channels.size()),
        .sampling_rate = config_.sampling_rate,
        .data_rate = uint32_t(sizeof(float) * channels.size() * config_.sampling_rate),
        .block_size = uint16_t(sizeof(float) * channels.size()),
        .bits_per_sample = uint16_t(8 * sizeof(float)),
        .extension = std::nullopt,
    };

    size_t max_channel_length = 0;
    for (auto& channel : channels) {
        max_channel_length = std::max(max_channel_length, channel.size());
    }

    auto format_chunk = std::make_unique<FmtWavChunk>(format);
    auto fact_chunk = std::make_unique<FactWavChunk>(channels.size() * max_channel_length);
    auto data_chunk = std::make_unique<DataWavChunk<float>>(std::move(channels));

    std::vector<std::unique_ptr<WavChunk>> chunks;
    chunks.push_back(std::move(format_chunk));
    chunks.push_back(std::move(fact_chunk));
    chunks.push_back(std::move(data_chunk));

    auto wave_chunk = std::make_unique<WaveWavChunk>(std::move(chunks));
    auto riff_chunk = std::make_unique<RiffWavChunk>(std::move(wave_chunk));

    riff_chunk->Encode(os);
}

}  // namespace wave_generator::model