project("wave-generator")

add_executable(${PROJECT_NAME}
    main.cc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    #synthesizer
    #model
    #view
    #imgui
    sdl2
    #spdlog_logger
    #glad2_loader
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION bin
)
