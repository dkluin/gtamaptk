add_executable(gtamaptk-assignlods gtamaptk-assignlods/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-assignlods FILES gtamaptk-assignlods/main.cpp)

target_include_directories(gtamaptk-assignlods PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-assignlods PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-assignlods RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})