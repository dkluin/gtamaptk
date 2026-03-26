set(GTAMAPTK_DUMP2DFX_SOURCE_LIST
    gtamaptk-dump2dfx/main.cpp
    
    gtamaptk-dump2dfx/librwgta/collisions.h
    gtamaptk-dump2dfx/librwgta/collisions.cpp
    gtamaptk-dump2dfx/librwgta/colourcodepipe.cpp
    gtamaptk-dump2dfx/librwgta/gtaplg.cpp
    gtamaptk-dump2dfx/librwgta/leedspipe.cpp
    gtamaptk-dump2dfx/librwgta/rsps2.cpp
    gtamaptk-dump2dfx/librwgta/rsxbox.cpp
    gtamaptk-dump2dfx/librwgta/rwgta.h)

add_executable(gtamaptk-dump2dfx ${GTAMAPTK_DUMP2DFX_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-dump2dfx FILES ${GTAMAPTK_DUMP2DFX_SOURCE_LIST})

target_include_directories(gtamaptk-dump2dfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-dump2dfx PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-dump2dfx PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-dump2dfx PUBLIC librw)

install(TARGETS gtamaptk-dump2dfx RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})