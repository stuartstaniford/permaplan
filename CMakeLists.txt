cmake_minimum_required(VERSION 3.17.1)
project (pmod)

SET(PMOD_COMPILE_FLAGS  "-g -Wall")
SET(PMOD_LINK_FLAGS     "-framework OpenGL -lglfw -lGLEW")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${PMOD_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${PMOD_LINK_FLAGS}")

include_directories(include)
include_directories(imgui)
include_directories(imgui/examples)
file(GLOB SOURCES "src/*.cpp")

add_executable(pmod ${SOURCES})
