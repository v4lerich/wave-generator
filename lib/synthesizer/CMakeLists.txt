project(wave-generator-synthesizer)

add_library(${PROJECT_NAME}
    signal_generator.h signal_generator.cc
    sine_generator.h sine_generator.cc
    constant_generator.h constant_generator.cc
    integrator.h integrator.cc
    )

add_library(wave-generator::synthesizer
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)
