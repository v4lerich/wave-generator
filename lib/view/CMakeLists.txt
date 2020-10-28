project(wave-generator-view)

add_library(${PROJECT_NAME}
    view.h demo_view.cpp demo_view.h)

add_library(wave-generator::view
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE imgui glad2_loader)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)