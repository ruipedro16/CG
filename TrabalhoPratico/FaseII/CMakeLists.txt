cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O2 -g")

project(engine)
set(ENGINE_FILES src/engine/engine.cpp src/engine/tinyxml2/tinyxml2.cpp src/engine/tinyxml2/tinyxml2.h
    src/common/vertex.cpp src/common/vertex.hpp src/engine/camera.cpp src/engine/camera.hpp
    src/engine/group.cpp src/engine/group.hpp src/engine/parser.cpp src/engine/parser.hpp
    src/engine/shape.cpp src/engine/shape.hpp src/engine/transform.cpp src/engine/transform.hpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
add_executable(engine ${ENGINE_FILES})
target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(generator)
set(GENERATOR_FILES src/generator/generator.cpp src/common/vertex.cpp src/common/vertex.hpp)
add_executable(generator ${GENERATOR_FILES})
