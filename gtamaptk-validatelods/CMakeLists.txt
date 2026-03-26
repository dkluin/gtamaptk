add_executable(gtamaptk-validatelods gtamaptk-validatelods/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-validatelods FILES gtamaptk-validatelods/main.cpp)

target_include_directories(gtamaptk-validatelods PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-validatelods PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-validatelods RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})