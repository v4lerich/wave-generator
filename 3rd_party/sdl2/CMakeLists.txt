project(sdl2)

if(TARGET ${PROJECT_NAME})
    return()
endif()

find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
    message("SDL2 found")
else()
    message(FATAL_ERROR "Cannot find SDL2")
endif()

add_library(${PROJECT_NAME}
    INTERFACE)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
    SDL2::SDL2
)
