add_executable(gtamaptk-entityinst gtamaptk-entityinst/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-entityinst FILES gtamaptk-entityinst/main.cpp)

target_include_directories(gtamaptk-entityinst PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-entityinst PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-entityinst RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})