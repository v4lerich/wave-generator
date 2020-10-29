project(wave-generator-synthesizer)

add_library(${PROJECT_NAME}
    signal_generator.h signal_generator.cc
    sine_generator.h sine_generator.cc
    sawtooth_generator.h sawtooth_generator.cc
    triangular_generator.h triangular_generator.cc
    amplitude_frequency_generator.h amplitude_frequency_generator.cc
    white_noise_generator.h white_noise_generator.cc
    pulse_generator.h pulse_generator.cc
    constant_generator.h constant_generator.cc
    integrator.h integrator.cc
    )

add_library(wave-generator::synthesizer
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)
