cmake_minimum_required(VERSION 3.10)
project(SoldatStdSrc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Find GLM
find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS /usr/local/include /usr/include)
if(GLM_INCLUDE_DIR)
    message(STATUS "Found GLM: ${GLM_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "GLM not found")
endif()

# Find GLEW
find_path(GLEW_INCLUDE_DIR GL/glew.h PATHS /usr/local/include /usr/include /usr/include/GL)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew libglew-2.1 PATHS /usr/local/lib /usr/lib /usr/lib/i386-linux-gnu)

if(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    set(GLEW_FOUND TRUE)
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    message(STATUS "Found GLEW: ${GLEW_LIBRARY}")
else()
    message(FATAL_ERROR "GLEW not found")
endif()

# Find GLU for OpenGL utility functions
find_library(GLU_LIBRARY NAMES GLU glu32 PATHS /usr/local/lib /usr/lib /usr/lib/i386-linux-gnu)
if(NOT GLU_LIBRARY)
    message(FATAL_ERROR "GLU library not found")
endif()

# Find PhysFS
find_path(PHYSFS_INCLUDE_DIR physfs.h PATHS /usr/local/include /usr/include)
find_library(PHYSFS_LIBRARY NAMES physfs PATHS /usr/local/lib /usr/lib)
if(PHYSFS_INCLUDE_DIR AND PHYSFS_LIBRARY)
    set(PHYSFS_FOUND TRUE)
    message(STATUS "Found PhysFS: ${PHYSFS_LIBRARY}")
    set(PHYSFS_LIBRARIES ${PHYSFS_LIBRARY})
    set(PHYSFS_INCLUDE_DIRS ${PHYSFS_INCLUDE_DIR})
else()
    message(WARNING "PhysFS not found - some features may be unavailable")
    set(PHYSFS_FOUND FALSE)
endif()

# Find SDL2 using different approach (optional)
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SDL2 sdl2)
    if(SDL2_FOUND)
        message(STATUS "Found SDL2 via pkg-config: ${SDL2_LIBRARIES}")
    else()
        message(WARNING "SDL2 not found via pkg-config")
    endif()
else()
    # Fallback: find SDL2 libraries manually
    find_path(SDL2_INCLUDE_DIR SDL.h PATHS /usr/local/include /usr/include)
    find_library(SDL2_LIBRARY NAMES SDL2 PATHS /usr/local/lib /usr/lib)
    if(SDL2_INCLUDE_DIR AND SDL2_LIBRARY)
        set(SDL2_FOUND TRUE)
        set(SDL2_LIBRARIES ${SDL2_LIBRARY})
        set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
        message(STATUS "Found SDL2: ${SDL2_LIBRARY}")
    else()
        message(WARNING "SDL2 not found")
        set(SDL2_FOUND FALSE)
    endif()
endif()

if(NOT SDL2_FOUND)
    message(WARNING "SDL2 not found - some features may be unavailable")
endif()

# Include directories - only the stdsrc directory itself
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIRS})
endif()
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

# Collect all header files from stdsrc
file(GLOB_RECURSE STD_SRC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Add the ClientMain.cpp for client executable
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ClientMain.cpp")
    file(GLOB CLIENT_SRC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Client*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Client*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Input.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GameRendering.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GostekGraphics.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/InterfaceGraphics.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/MapGraphics.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/FileClient.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/FileClient.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ClientCommands.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GameMenus.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/LobbyClient.h"
    )
    
    add_executable(soldat_client
        "${CMAKE_CURRENT_SOURCE_DIR}/ClientMain.cpp"
        ${CLIENT_SRC}
        ${STD_SRC_HEADERS}
    )
    
    if(SDL2_FOUND)
        set(CLIENT_LIBS
            OpenGL::GL
            ${GLU_LIBRARY}
            glfw
            ${GLEW_LIBRARIES}
        )
        if(PHYSFS_FOUND)
            list(APPEND CLIENT_LIBS ${PHYSFS_LIBRARY})
            target_include_directories(soldat_client PRIVATE ${PHYSFS_INCLUDE_DIRS})
        endif()
        list(APPEND CLIENT_LIBS ${SDL2_LIBRARIES})
        target_include_directories(soldat_client PRIVATE ${SDL2_INCLUDE_DIRS})
        
        target_compile_definitions(soldat_client PRIVATE CLIENT_CODE)
        target_link_libraries(soldat_client ${CLIENT_LIBS})
        message(STATUS "Built soldat_client executable")
    else()
        message(WARNING "SDL2 not found - skipping soldat_client executable")
    endif()
else()
    message(WARNING "ClientMain.cpp not found - skipping soldat_client executable")
endif()

# Add the ServerMain.cpp and associated files for server executable
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ServerMain.cpp")
    file(GLOB SERVER_SRC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Server*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Server*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*Commands*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*Commands*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/BanSystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/BanSystem.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/FileServer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ServerLoop.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ServerHelper.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ServerHelper.h"
    )
    
    add_executable(soldat_server
        "${CMAKE_CURRENT_SOURCE_DIR}/ServerMain.cpp"
        ${SERVER_SRC}
        ${STD_SRC_HEADERS}
    )
    
    target_compile_definitions(soldat_server PRIVATE SERVER_CODE)
    set(SERVER_LIBS
        OpenGL::GL
        ${GLU_LIBRARY}
        glfw
        ${GLEW_LIBRARIES}
    )
    if(PHYSFS_FOUND)
        list(APPEND SERVER_LIBS ${PHYSFS_LIBRARY})
        target_include_directories(soldat_server PRIVATE ${PHYSFS_INCLUDE_DIRS})
    endif()
    if(SDL2_FOUND)
        list(APPEND SERVER_LIBS ${SDL2_LIBRARIES})
        target_include_directories(soldat_server PRIVATE ${SDL2_INCLUDE_DIRS})
    endif()
    
    target_link_libraries(soldat_server ${SERVER_LIBS})
    message(STATUS "Built soldat_server executable")
else()
    message(WARNING "ServerMain.cpp not found - skipping soldat_server executable")
endif()

message(STATUS "Build system configured for stdsrc directory")