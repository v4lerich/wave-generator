project(imgui_filedialog)

if(TARGET ${PROJECT_NAME})
    return()
endif()

file(GLOB ${PROJECT_NAME}_SOURCES
    imgui_filedialog_config.h
    ImGuiFileDialog/ImGuiFileDialog/*.h
    ImGuiFileDialog/ImGuiFileDialog/*.cpp)


add_library(${PROJECT_NAME}
    STATIC ${${PROJECT_NAME}_SOURCES})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD2
    PRIVATE CUSTOM_IMGUIFILEDIALOG_CONFIG=<imgui_filedialog_config.h>
    PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

target_include_directories(${PROJECT_NAME}
    PUBLIC ImGuiFileDialog/ImGuiFileDialog/ .)

target_link_libraries(${PROJECT_NAME}
    PUBLIC sdl2 glad2-loader imgui icon-font-headers dirent)
