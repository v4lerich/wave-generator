project(wave-generator-view)

add_library(${PROJECT_NAME}
    view.cc view.h
    synthesizer_view.cc synthesizer_view.h
    editor_view.cc editor_view.h
    node/node_view.cc node/node_view.h
    node/node_port_view.cc node/node_port_view.h
    node/node_views.cc node/node_views.h
    node/node_port_views.cc node/node_port_views.h
    node/node_view_factory.cc node/node_view_factory.h
    player_view.cc player_view.h)

add_library(wave-generator::view
    ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE icon-font-headers
    PUBLIC imgui wave-generator::synthesizer wave-generator::model)

target_include_directories(${PROJECT_NAME}
    PUBLIC .)