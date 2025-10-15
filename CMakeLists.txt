cmake_minimum_required(VERSION 3.25)
project(game)
message("Source dir: ${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

# add other OS
if(WIN32)
  set(LIBS glfw3 opengl32)
#   add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

# add other compilers
if (NOT MSVC)
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        # -Werror
    )
elseif (MSVC)
    add_compile_options(
        /W4
        # /WX
    )
endif()

add_library(GLAD "libs/glad/glad.c")
set(LIBS ${LIBS} GLAD)

add_executable(game 
    src/main.cpp
    src/utils.cpp
    src/shader.cpp
)

target_link_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/GLFW
    ${CMAKE_SOURCE_DIR}/libs/assimp
)

include_directories(${CMAKE_SOURCE_DIR}/includes)
target_link_libraries(game PRIVATE ${LIBS})
# target_include_directories(CMAKE_PROJECT_NAME PRIVATE includes)
# target_link_libraries(CMAKE_PROJECT_NAME ${LIBS})