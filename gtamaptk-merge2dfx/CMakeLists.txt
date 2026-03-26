set(GTAMAPTK_MERGE2DFX_SOURCE_LIST
    gtamaptk-merge2dfx/main.cpp
    
    gtamaptk-merge2dfx/librwgta/collisions.h
    gtamaptk-merge2dfx/librwgta/collisions.cpp
    gtamaptk-merge2dfx/librwgta/colourcodepipe.cpp
    gtamaptk-merge2dfx/librwgta/gtaplg.cpp
    gtamaptk-merge2dfx/librwgta/leedspipe.cpp
    gtamaptk-merge2dfx/librwgta/rsps2.cpp
    gtamaptk-merge2dfx/librwgta/rsxbox.cpp
    gtamaptk-merge2dfx/librwgta/rwgta.h)

add_executable(gtamaptk-merge2dfx ${GTAMAPTK_MERGE2DFX_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-merge2dfx FILES ${GTAMAPTK_MERGE2DFX_SOURCE_LIST})

target_include_directories(gtamaptk-merge2dfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-merge2dfx PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-merge2dfx PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-merge2dfx PUBLIC librw)

install(TARGETS gtamaptk-merge2dfx RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})