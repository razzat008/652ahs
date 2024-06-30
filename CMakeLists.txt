cmake_minimum_required(VERSION 3.14)

project(652ahs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenGL
find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
set(SQLITE_DIR ${CMAKE_SOURCE_DIR}/libs/sqlite)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/libs/glfw)
set(IMGUI_INCLUDE_DIR ${IMGUI_DIR} ${IMGUI_DIR}/backends)
# set(IMGUI_INCLUDE_DIR ${SQLITE_DIR} ${SQLITE_DIR}/)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp)
set(IMGUI_IMPL_SOURCES ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
                       ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCES src/*.cpp ${IMGUI_SOURCES} ${IMGUI_IMPL_SOURCES})
add_subdirectory(${GLFW_DIR})

# Create the executable
add_executable(652ahs ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_IMPL_SOURCES})

# Link libraries
target_link_libraries(652ahs PRIVATE glfw OpenGL::GL sqlite3)

# Include directories
target_include_directories(
  652ahs PRIVATE ${CMAKE_SOURCE_DIR}/include ${IMGUI_INCLUDE_DIR}
                 ${BOOST_ASIO_INCLUDE_DIR})
