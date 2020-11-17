#ifndef WAVEGENERATOR_WAV_FORMAT_H
#define WAVEGENERATOR_WAV_FORMAT_H

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

namespace wave_generator::model {

template <typename T>
using WavSignalChannel = std::vector<T>;

class WavFormatEncoder {
  public:
    struct Config {
        uint32_t sampling_rate;
    };
    explicit WavFormatEncoder(Config config);

    void Encode(std::ostream& ostream, std::vector<WavSignalChannel<float>> channels);

  private:
    Config config_;
};

struct WavFormat {
    using Uuid = std::array<uint8_t, 16>;
    enum Code : uint16_t {
        kWaveFormatPcm = 0x0001,
        kWaveFormatIeeeFloat = 0x0003,
        kWaveFormatAlaw = 0x0006,
        kWaveFormatMuLaw = 0x0007,
        kWaveFormatExtensible = 0xfffe,
    };
    struct Extension {
        uint16_t valid_bits_per_sample{};
        uint32_t channel_mask{};
        Uuid sub_format{};
    };

    Code format_tag{};
    uint16_t channels{};
    uint32_t sampling_rate{};
    uint32_t data_rate{};
    uint16_t block_size{};
    uint16_t bits_per_sample{};
    std::optional<Extension> extension{};
};

class WavChunk {
  public:
    explicit WavChunk(std::string chunk_id);

    virtual void Encode(std::ostream& os) const;

  protected:
    template <typename T>
    static void Encode(std::ostream& os, const T& value);

  private:
    std::string chunk_id_;
};

class FmtWavChunk : public WavChunk {
  public:
    explicit FmtWavChunk(WavFormat format);

    void Encode(std::ostream& os) const override;

  private:
    WavFormat format_;
};

template <typename T>
class DataWavChunk : public WavChunk {
  public:
    explicit DataWavChunk(std::vector<WavSignalChannel<T>> channels);
    void Encode(std::ostream& os) const override;

  private:
    std::vector<WavSignalChannel<T>> channels_;
};

class WaveWavChunk : public WavChunk {
  public:
    explicit WaveWavChunk(std::vector<std::unique_ptr<WavChunk>> chunks);

    void Encode(std::ostream& os) const override;

  private:
    std::vector<std::unique_ptr<WavChunk>> chunks_;
};

class RiffWavChunk : public WavChunk {
  public:
    explicit RiffWavChunk(std::unique_ptr<WaveWavChunk> wave_chunk);

    void Encode(std::ostream& os) const override;

  private:
    std::unique_ptr<WavChunk> wave_chunk_;
};

}  // namespace wave_generator::model

#endif  // WAVEGENERATOR_WAV_FORMAT_H
