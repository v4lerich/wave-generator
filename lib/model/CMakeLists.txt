project(wave-generator-model)

add_library(${PROJECT_NAME}
    sound_device.cc sound_device.h signal_samples_generator.cc signal_samples_generator.h wav_format.cc wav_format.h sound_recorder.cc sound_recorder.h)

add_library(wave-generator::model
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sdl2 wave-generator::synthesizer)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)