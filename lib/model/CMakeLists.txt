project(wave-generator-model)

add_library(${PROJECT_NAME}
    sound_device.cc sound_device.h sound_generator.cc sound_generator.h)

add_library(wave-generator::model
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sdl2 wave-generator::synthesizer)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)