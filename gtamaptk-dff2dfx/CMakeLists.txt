add_executable(gtamaptk-dff2dfx gtamaptk-dff2dfx/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-dff2dfx FILES gtamaptk-dff2dfx/main.cpp)

target_include_directories(gtamaptk-dff2dfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-dff2dfx PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-dff2dfx RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})