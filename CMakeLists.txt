cmake_minimum_required(VERSION 3.11)
project(phi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Sources

set(SOURCE_FILES main.cpp)
set(ENGINE_SOURCES engine/engine.cpp)
set(CORE_SOURCES engine/core/tf.cpp)
set(SYS_SOURCES engine/sys/event.cpp)
set(PHIGL_SOURCES engine/gl/shader.cpp)
set(RENDER_SOURCES engine/render/compositor.cpp engine/render/mesh.cpp)

include_directories(include)

### Libraries and executables

add_executable(phigame ${SOURCE_FILES})

add_library(phi ${ENGINE_SOURCES} ${CORE_SOURCES} ${SYS_SOURCES} ${PHIGL_SOURCES} ${RENDER_SOURCES})
#add_library(phiphys)

### Compiler and Linker settings

if (MSVC)
    # Force to always compile with W4
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")
endif ()

if (WIN32)
    if (MSVC)
        get_property(TEMP TARGET phigame PROPERTY LINK_FLAGS)
        set_target_properties(phigame PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS ${TEMP}")
        get_property(TEMP TARGET phigame PROPERTY LINK_FLAGS_DEBUG)
        set_target_properties(phigame PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE ${TEMP}")
    elseif (CYGWIN OR MINGW OR MSYS)
        # Required to get log output
        target_link_libraries(phigame debug -mconsole)
        target_link_libraries(phigame optimized -mwindows)
    endif ()
endif ()

### Dependencies

include_directories(vendor)

add_definitions(-DRMT_USE_OPENGL=1)
add_library(remotery vendor/Remotery/lib/Remotery.c)
if (UNIX)
    target_link_libraries(remotery pthread)
endif ()
if (WIN32)
    target_link_libraries(remotery ws2_32.lib)
endif ()
include_directories(vendor/Remotery/lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
include_directories(vendor/glfw/include)

add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(vendor/glew)
include_directories(vendor/glew/include vendor/glew/src)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(vendor/glm)

target_link_libraries(phi remotery glfw ${GLFW_LIBRARIES} libglew_static)

target_link_libraries(phigame phi)
