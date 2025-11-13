cmake_minimum_required(VERSION 3.10)
project(Soldat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_SERVER "Build server executable" OFF)
option(BUILD_CLIENT "Build client executable" ON)

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
    message(STATUS "Found PhysFS: ${PHYSFS_LIBRARY}")
else()
    message(FATAL_ERROR "PhysFS not found")
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message(STATUS "Found SDL2")
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/stdsrc)
include_directories(${GLM_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${PHYSFS_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

# Collect source files
file(GLOB_RECURSE STD_SRC_FILES "${PROJECT_SOURCE_DIR}/soldat/stdsrc/*.h")

# Define common libraries
set(COMMON_LIBRARIES
    OpenGL::GL
    ${GLU_LIBRARY}
    glfw
    ${GLEW_LIBRARIES}
    ${PHYSFS_LIBRARY}
    ${SDL2_LIBRARIES}
)

# Client executable
if(BUILD_CLIENT)
    add_executable(soldat_client
        src/main.cpp
        src/client_main.cpp
        ${STD_SRC_FILES}
    )
    
    target_compile_definitions(soldat_client PRIVATE CLIENT_CODE)
    target_link_libraries(soldat_client ${COMMON_LIBRARIES})
    target_include_directories(soldat_client PRIVATE 
        ${PROJECT_SOURCE_DIR}/soldat/stdsrc
        ${PROJECT_SOURCE_DIR}/soldat/pascal/client)
endif()

# Server executable
if(BUILD_SERVER)
    add_executable(soldat_server
        src/server_main.cpp
        ${STD_SRC_FILES}
    )
    
    target_compile_definitions(soldat_server PRIVATE SERVER_CODE)
    target_link_libraries(soldat_server ${COMMON_LIBRARIES})
    target_include_directories(soldat_server PRIVATE 
        ${PROJECT_SOURCE_DIR}/soldat/stdsrc
        ${PROJECT_SOURCE_DIR}/soldat/pascal/server)
endif()

if(NOT BUILD_CLIENT AND NOT BUILD_SERVER)
    message(WARNING "Neither client nor server selected for building. Building client by default.")
    add_executable(soldat_client
        src/main.cpp
        src/client_main.cpp
        ${STD_SRC_FILES}
    )
    
    target_compile_definitions(soldat_client PRIVATE CLIENT_CODE)
    target_link_libraries(soldat_client ${COMMON_LIBRARIES})
    target_include_directories(soldat_client PRIVATE 
        ${PROJECT_SOURCE_DIR}/soldat/stdsrc
        ${PROJECT_SOURCE_DIR}/soldat/pascal/client)
endif()

# Copy assets to build directory
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
file(COPY configs/ DESTINATION ${CMAKE_BINARY_DIR}/configs)
file(COPY maps/ DESTINATION ${CMAKE_BINARY_DIR}/maps)