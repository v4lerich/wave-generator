project(wave-generator-view)

add_library(${PROJECT_NAME}
    view.cc view.h
    synthesizer_view.cc synthesizer_view.h
    editor_view.cc editor_view.h
    node/node_view.cc node/node_view.h
    node/node_port_view.cc node/node_port_view.h
    node/node_views.cc node/node_views.h
    node/node_port_views.cc node/node_port_views.h)

add_library(wave-generator::view
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE imgui glad2_loader
    PUBLIC wave-generator::synthesizer)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)