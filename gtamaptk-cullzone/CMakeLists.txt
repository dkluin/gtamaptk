add_executable(gtamaptk-cullzone gtamaptk-cullzone/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-cullzone FILES gtamaptk-cullzone/main.cpp)

target_include_directories(gtamaptk-cullzone PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-cullzone PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-cullzone RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})