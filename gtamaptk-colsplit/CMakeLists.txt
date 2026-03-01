set(GTAMAPTK_COLSPLIT_SOURCE_LIST
    gtamaptk-colsplit/main.cpp)

add_executable(gtamaptk-colsplit ${GTAMAPTK_COLSPLIT_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-colsplit FILES ${GTAMAPTK_COLSPLIT_SOURCE_LIST})

target_include_directories(gtamaptk-colsplit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-colsplit PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-colsplit PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-colsplit PUBLIC librw)

install(TARGETS gtamaptk-colsplit RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})