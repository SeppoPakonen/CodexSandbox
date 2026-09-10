cmake_minimum_required(VERSION 3.10)
project(PinballGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Create the executable
add_executable(pinball_game
    pinball/src/main.cpp
    pinball/src/renderer.cpp
    pinball/src/shader.cpp
    pinball/src/camera.cpp
    pinball/src/input.cpp
    pinball/src/ball.cpp
    pinball/src/paddle.cpp
    pinball/src/game.cpp
    pinball/src/simple_renderer.cpp
)

# Include directories
target_include_directories(pinball_game PRIVATE pinball/include)
target_include_directories(pinball_game PRIVATE ${GLM_INCLUDE_DIR})
target_include_directories(pinball_game PRIVATE ${GLEW_INCLUDE_DIRS})

# Link libraries
target_link_libraries(pinball_game 
    OpenGL::GL 
    ${GLU_LIBRARY}
    glfw
    ${GLEW_LIBRARIES}
)

# Copy shaders to build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/pinball/shaders)
    file(COPY ${CMAKE_SOURCE_DIR}/pinball/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/shaders)
    file(COPY ${CMAKE_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)
else()
    message(WARNING "Shaders directory not found, creating empty directory")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
endif()