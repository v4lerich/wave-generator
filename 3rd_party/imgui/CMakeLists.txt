project(imgui)

if(TARGET ${PROJECT_NAME})
    return()
endif()

add_subdirectory(external/sdl2)
add_subdirectory(external/glad2_loader)

add_library(${PROJECT_NAME}
    STATIC
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl.h
    imgui/backends/imgui_impl_sdl.cpp)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD2)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    imgui)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sdl2 glad2_loader)
