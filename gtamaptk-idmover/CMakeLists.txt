add_executable(gtamaptk-idmover gtamaptk-idmover/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-idmover FILES gtamaptk-idmover/main.cpp)

target_include_directories(gtamaptk-idmover PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-idmover PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-idmover RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})