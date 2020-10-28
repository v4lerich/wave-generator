project(spdlog_logger)

if(TARGET ${PROJECT_NAME})
    return()
endif()

add_subdirectory(spdlog)

add_library(${PROJECT_NAME}
    INTERFACE)

target_link_libraries(${PROJECT_NAME}
    INTERFACE spdlog::spdlog)
