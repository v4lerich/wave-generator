#ifndef WAVEGENERATOR_APPLICATION_CONFIG_H
#define WAVEGENERATOR_APPLICATION_CONFIG_H

#include <SDL_log.h>
#include <IconsForkAwesome.h>

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
const std::string kResourcePath = "../res/";
const std::string kRubikFontPath = kResourcePath + "RubikFont/Rubik-Medium.ttf";
const std::string kForkAwesomeFontPath = kResourcePath + "ForkAwesomeFont/" + FONT_ICON_FILE_NAME_FK;

constexpr auto kLogLevel =
#ifdef NDEBUG
    SDL_LogPriority::SDL_LOG_PRIORITY_WARN;
#else
    SDL_LogPriority::SDL_LOG_PRIORITY_INFO;
#endif

}

#endif  // WAVEGENERATOR_APPLICATION_CONFIG_H
