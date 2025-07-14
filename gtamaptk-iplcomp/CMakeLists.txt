add_executable(gtamaptk-iplcomp gtamaptk-iplcomp/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-iplcomp FILES gtamaptk-iplcomp/main.cpp)

target_include_directories(gtamaptk-iplcomp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-iplcomp PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-iplcomp RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})