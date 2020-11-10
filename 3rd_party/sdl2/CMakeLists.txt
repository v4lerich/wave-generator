project(sdl2)

if(TARGET ${PROJECT_NAME})
    return()
endif()

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_library(${PROJECT_NAME}
    INTERFACE)

target_link_libraries(${PROJECT_NAME}
    INTERFACE SDL2-static)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE SDL_MAIN_HANDLED)
