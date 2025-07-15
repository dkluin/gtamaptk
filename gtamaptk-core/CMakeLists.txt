add_library(gtamaptk-core
    gtamaptk-core/gtamaptk-core.h
    gtamaptk-core/IDE/section/Ide2dfxEntry.h
    gtamaptk-core/IDE/section/Ide2dfxEntry.cpp
    gtamaptk-core/IDE/section/IdeAnimEntry.h
    gtamaptk-core/IDE/section/IdeAnimEntry.cpp
    gtamaptk-core/IDE/section/IdeObjsEntry.h
    gtamaptk-core/IDE/section/IdeObjsEntry.cpp
    gtamaptk-core/IDE/section/IdeTobjEntry.h
    gtamaptk-core/IDE/section/IdeTobjEntry.cpp
    gtamaptk-core/IDE/section/IdeTxdpEntry.h
    gtamaptk-core/IDE/section/IdeTxdpEntry.cpp
    gtamaptk-core/IDE/IdeFile.h
    gtamaptk-core/IDE/IdeFile.cpp
    gtamaptk-core/IPL/section/IplAuzoEntry.h
    gtamaptk-core/IPL/section/IplAuzoEntry.cpp
    gtamaptk-core/IPL/section/IplCarsEntry.h
    gtamaptk-core/IPL/section/IplCarsEntry.cpp
    gtamaptk-core/IPL/section/IplCullEntry.h
    gtamaptk-core/IPL/section/IplCullEntry.cpp
    gtamaptk-core/IPL/section/IplEnexEntry.h
    gtamaptk-core/IPL/section/IplEnexEntry.cpp
    gtamaptk-core/IPL/section/IplGrgeEntry.h
    gtamaptk-core/IPL/section/IplGrgeEntry.cpp
    gtamaptk-core/IPL/section/IplInstEntry.h
    gtamaptk-core/IPL/section/IplInstEntry.cpp
    gtamaptk-core/IPL/section/IplJumpEntry.h
    gtamaptk-core/IPL/section/IplJumpEntry.cpp
    gtamaptk-core/IPL/section/IplOcclEntry.h
    gtamaptk-core/IPL/section/IplOcclEntry.cpp
    gtamaptk-core/IPL/section/IplPickEntry.h
    gtamaptk-core/IPL/section/IplPickEntry.cpp
    gtamaptk-core/IPL/section/IplTcycEntry.h
    gtamaptk-core/IPL/section/IplTcycEntry.cpp
    gtamaptk-core/IPL/section/IplZoneEntry.h
    gtamaptk-core/IPL/section/IplZoneEntry.cpp
    gtamaptk-core/IPL/IplFile.h
    gtamaptk-core/IPL/IplFile.cpp
    gtamaptk-core/cullzone/CullZoneFile.h
    gtamaptk-core/cullzone/CullZoneFile.cpp
    gtamaptk-core/main/GameMap.h
    gtamaptk-core/main/GameMap.cpp
    gtamaptk-core/main/MapFileSection.h
    gtamaptk-core/main/Shared.hpp
    gtamaptk-core/main/Shared.cpp
    gtamaptk-core/math/Box.h
    gtamaptk-core/math/Misc.h
    gtamaptk-core/math/Quaternion.h
    gtamaptk-core/math/RGBA.h
    gtamaptk-core/math/Sphere.h
    gtamaptk-core/math/Vector2d.h
    gtamaptk-core/math/Vector3d.h)

set_target_properties(gtamaptk-core
    PROPERTIES
        C_STANDARD 11
        C_EXTENSIONS OFF
        C_STANDARD_REQUIRED ON
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        PREFIX "")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gtamaptk-core)