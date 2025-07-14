set(GTAMAPTK_BUILDTXDFOLDERS_SOURCE_LIST
    gtamaptk-buildtxdfolders/main.cpp
    
    gtamaptk-buildtxdfolders/librwgta/collisions.h
    gtamaptk-buildtxdfolders/librwgta/collisions.cpp
    gtamaptk-buildtxdfolders/librwgta/colourcodepipe.cpp
    gtamaptk-buildtxdfolders/librwgta/gtaplg.cpp
    gtamaptk-buildtxdfolders/librwgta/leedspipe.cpp
    gtamaptk-buildtxdfolders/librwgta/rsps2.cpp
    gtamaptk-buildtxdfolders/librwgta/rsxbox.cpp
    gtamaptk-buildtxdfolders/librwgta/rwgta.h)

add_executable(gtamaptk-buildtxdfolders ${GTAMAPTK_BUILDTXDFOLDERS_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-buildtxdfolders FILES ${GTAMAPTK_BUILDTXDFOLDERS_SOURCE_LIST})

target_include_directories(gtamaptk-buildtxdfolders PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-buildtxdfolders PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-buildtxdfolders PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-buildtxdfolders PUBLIC librw)

install(TARGETS gtamaptk-buildtxdfolders RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})