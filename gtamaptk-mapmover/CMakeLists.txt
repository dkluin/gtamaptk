add_executable(gtamaptk-mapmover gtamaptk-mapmover/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-mapmover FILES gtamaptk-mapmover/main.cpp)

target_include_directories(gtamaptk-mapmover PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-mapmover PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-mapmover RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})