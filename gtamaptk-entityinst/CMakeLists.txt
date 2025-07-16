set(GTAMAPTK_ENTITYINST_SOURCE_LIST
    gtamaptk-entityinst/main.cpp
    
    gtamaptk-entityinst/inst/classdata/BaseEntityClassData1.h
    gtamaptk-entityinst/inst/classdata/BaseEntityClassData1.cpp
    gtamaptk-entityinst/inst/classdata/BaseEntityClassData2.h
    gtamaptk-entityinst/inst/classdata/BaseEntityClassData2.cpp
    gtamaptk-entityinst/inst/classdata/CameraEntityClassData.h
    gtamaptk-entityinst/inst/classdata/CameraEntityClassData.cpp
    gtamaptk-entityinst/inst/classdata/DoorEntityClassData1.h
    gtamaptk-entityinst/inst/classdata/DoorEntityClassData1.cpp
    gtamaptk-entityinst/inst/classdata/DoorEntityClassData2.h
    gtamaptk-entityinst/inst/classdata/DoorEntityClassData2.cpp
    gtamaptk-entityinst/inst/classdata/HunterEntityClassData1.h
    gtamaptk-entityinst/inst/classdata/HunterEntityClassData1.cpp
    gtamaptk-entityinst/inst/classdata/HunterEntityClassData2.h
    gtamaptk-entityinst/inst/classdata/HunterEntityClassData2.cpp
    gtamaptk-entityinst/inst/classdata/LightEntityClassData1.h
    gtamaptk-entityinst/inst/classdata/LightEntityClassData1.cpp
    gtamaptk-entityinst/inst/classdata/LightEntityClassData2.h
    gtamaptk-entityinst/inst/classdata/LightEntityClassData2.cpp
    gtamaptk-entityinst/inst/classdata/PlayerEntityClassData.h
    gtamaptk-entityinst/inst/classdata/PlayerEntityClassData.cpp
    gtamaptk-entityinst/inst/classdata/TriggerEntityClassData.h
    gtamaptk-entityinst/inst/classdata/TriggerEntityClassData.cpp
    gtamaptk-entityinst/inst/EntityInstance.h
    gtamaptk-entityinst/inst/EntityInstance.cpp
    gtamaptk-entityinst/inst/EntityInstFile.h
    gtamaptk-entityinst/inst/EntityInstFile.cpp)

add_executable(gtamaptk-entityinst ${GTAMAPTK_ENTITYINST_SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-entityinst FILES ${GTAMAPTK_ENTITYINST_SOURCE_LIST})

target_include_directories(gtamaptk-entityinst PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)
target_link_libraries(gtamaptk-entityinst PRIVATE gtamaptk-core)

install(TARGETS gtamaptk-entityinst RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

