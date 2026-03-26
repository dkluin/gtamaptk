set(GTAMAPTK_CONV2DFX_SOURCE_LIST
    gtamaptk-conv2dfx/main.cpp
    
    gtamaptk-conv2dfx/librwgta/collisions.h
    gtamaptk-conv2dfx/librwgta/collisions.cpp
    gtamaptk-conv2dfx/librwgta/colourcodepipe.cpp
    gtamaptk-conv2dfx/librwgta/gtaplg.cpp
    gtamaptk-conv2dfx/librwgta/leedspipe.cpp
    gtamaptk-conv2dfx/librwgta/rsps2.cpp
    gtamaptk-conv2dfx/librwgta/rsxbox.cpp
    gtamaptk-conv2dfx/librwgta/rwgta.h)

add_executable(gtamaptk-conv2dfx ${GTAMAPTK_CONV2DFX_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-conv2dfx FILES ${GTAMAPTK_CONV2DFX_SOURCE_LIST})

target_include_directories(gtamaptk-conv2dfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-conv2dfx PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-conv2dfx PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-conv2dfx PUBLIC librw)

install(TARGETS gtamaptk-conv2dfx RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})