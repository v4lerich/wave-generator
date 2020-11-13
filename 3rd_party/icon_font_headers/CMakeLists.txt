project(icon-font-headers
    LANGUAGES C)

if (TARGET ${PROJECT_NAME})
    return()
endif()

add_library(${PROJECT_NAME}
    INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE IconFontCppHeaders)