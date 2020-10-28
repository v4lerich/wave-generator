#ifndef WAVEGENERATOR_APPLICATION_CONFIG_H
#define WAVEGENERATOR_APPLICATION_CONFIG_H

#include <spdlog/spdlog.h>

namespace wave_generator::config {

enum class ApplicationTheme {
    kClassic,
    kLight,
    kDark
};

struct OpenGlVersion {
    int major;
    int minor;
};

constexpr auto kWindowWidth = 1280;
constexpr auto kWindowHeight = 720;

constexpr auto kVsyncEnable = true;
constexpr auto kImGuiTheme = ApplicationTheme::kClassic;

constexpr auto kOpenGlVersion = OpenGlVersion { .major = 4, .minor = 2 };
constexpr auto kGlslVersion = "#version 420";

constexpr auto kLogLevel =
#ifndef NDEBUG
    spdlog::level::info;
#else
    spdlog::level::trace;
#endif

}

#endif  // WAVEGENERATOR_APPLICATION_CONFIG_H
