project("wave-generator")

add_executable(${PROJECT_NAME}
    main.cc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    imgui
    sdl2
    glad2_loader
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION bin
)
