project(glad2_loader)

if (TARGET ${PROJECT_NAME}) 
    return()
endif()

add_subdirectory(glad/cmake)
glad_add_library(glad_gl_core_42
    REPRODUCIBLE STATIC ALIAS
    LANGUAGE "c++"
    API gl:core=4.2)

add_library(${PROJECT_NAME}
    include/glad2_loader.h
    src/glad2_loader.cc)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sdl2
    PUBLIC glad_gl_core_42)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)
