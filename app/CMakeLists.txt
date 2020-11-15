project(wave-generator)

add_executable(${PROJECT_NAME}
    main.cc
    application.cc application.h
    application_config.h)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    imgui
    sdl2
    glad2-loader
    wave-generator::view
    icon-font-headers
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION bin
)
