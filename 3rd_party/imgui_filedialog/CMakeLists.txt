project(imgui_filedialog)

if(TARGET ${PROJECT_NAME})
    return()
endif()

file(GLOB ${PROJECT_NAME}_SOURCES
    imgui_filedialog_config.h
    ImGuiFileDialog/ImGuiFileDialog/*.h
    ImGuiFileDialog/ImGuiFileDialog/*.cpp)

if(WIN32)
    set(DIRENT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/dirent/dirent/include)
    include_directories(BEFORE SYSTEM ${DIRENT_INCLUDE_DIR})
endif()


add_library(${PROJECT_NAME}
    STATIC ${${PROJECT_NAME}_SOURCES})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC CUSTOM_IMGUIFILEDIALOG_CONFIG=<imgui_filedialog_config.h>)

target_include_directories(${PROJECT_NAME}
    PUBLIC ImGuiFileDialog/ImGuiFileDialog/ .)

target_link_libraries(${PROJECT_NAME}
    PUBLIC sdl2 glad2-loader imgui icon-font-headers)
