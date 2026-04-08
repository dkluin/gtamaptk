set(GTAMAPTK_BREAKABLE_SOURCE_LIST
    gtamaptk-breakable/main.cpp
    
    gtamaptk-breakable/librwgta/collisions.h
    gtamaptk-breakable/librwgta/collisions.cpp
    gtamaptk-breakable/librwgta/colourcodepipe.cpp
    gtamaptk-breakable/librwgta/gtaplg.cpp
    gtamaptk-breakable/librwgta/leedspipe.cpp
    gtamaptk-breakable/librwgta/rsps2.cpp
    gtamaptk-breakable/librwgta/rsxbox.cpp
    gtamaptk-breakable/librwgta/rwgta.h)

add_executable(gtamaptk-breakable ${GTAMAPTK_BREAKABLE_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-breakable FILES ${GTAMAPTK_BREAKABLE_SOURCE_LIST})

target_include_directories(gtamaptk-breakable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_include_directories(gtamaptk-breakable PRIVATE ${LIBRW_INCLUDE_PATH})

target_link_libraries(gtamaptk-breakable PRIVATE gtamaptk-core)
target_link_libraries(gtamaptk-breakable PUBLIC librw)

install(TARGETS gtamaptk-breakable RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})