# File:       CMakeLists.txt
# Owner:      Sara
# Purpose:    Builds the resqgrid program and the 4 test programs, links SFML 3
#             for the GUI, turns warnings on.
# Depends on: CMake >= 3.16, a C++17 compiler, SFML 3.0 (optional: GUI only)
# Used by:    everyone ("one command builds everything")
# Milestone:  M0
#
#   cmake -S . -B build            (add -DRESQGRID_GUI=OFF to skip SFML)
#   cmake --build build
#   ctest --test-dir build         (runs the 4 tests)
cmake_minimum_required(VERSION 3.16)
project(ResQGrid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# --- warnings on for all our targets ---------------------------------------
function(resqgrid_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# --- core library: everything except main.cpp and the UI --------------------
# Containers are header-only; listed so IDEs show them.
set(RESQGRID_CORE_SOURCES
    src/core/Robot.cpp
    src/core/Request.cpp
    src/core/Environment.cpp
    src/core/Router.cpp
    src/core/ScenarioLoader.cpp
    src/core/EventProcessor.cpp
    src/core/Rerouter.cpp
    src/core/Coordinator.cpp
    src/core/Movement.cpp
    src/core/Simulation.cpp
    src/output/Logger.cpp
    src/output/Statistics.cpp
    src/ui/GraphLayout.cpp           # no SFML inside: always built
)
set(RESQGRID_HEADERS
    src/containers/DynamicArray.h src/containers/LinkedList.h src/containers/Queue.h
    src/containers/MergeSort.h src/containers/HashMap.h src/containers/MinHeap.h
    src/containers/IndexedMaxHeap.h src/core/Types.h src/core/World.h
)
add_library(resqgrid_core STATIC ${RESQGRID_CORE_SOURCES} ${RESQGRID_HEADERS})
target_include_directories(resqgrid_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
resqgrid_warnings(resqgrid_core)

# --- SFML 3 (GUI) ------------------------------------------------------------
option(RESQGRID_GUI "Build the SFML 3 window (--gui)" ON)
set(RESQGRID_HAS_GUI OFF)
if(RESQGRID_GUI)
    find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
    if(SFML_FOUND)
        set(RESQGRID_HAS_GUI ON)
        message(STATUS "ResQGrid: SFML ${SFML_VERSION} found, GUI enabled")
    else()
        message(WARNING "ResQGrid: SFML 3 not found - building WITHOUT the GUI. "
                        "Install SFML 3 (see README) or pass -DSFML_DIR=<path>/lib/cmake/SFML.")
    endif()
endif()

# --- the program ---------------------------------------------------------------
add_executable(resqgrid src/main.cpp)
target_link_libraries(resqgrid PRIVATE resqgrid_core)
resqgrid_warnings(resqgrid)
if(RESQGRID_HAS_GUI)
    target_sources(resqgrid PRIVATE src/ui/Renderer.cpp src/ui/UIApp.cpp)
    target_compile_definitions(resqgrid PRIVATE RESQGRID_HAS_GUI)
    target_link_libraries(resqgrid PRIVATE SFML::Graphics SFML::Window SFML::System)
endif()

# Logs go to output/ (gitignored except .gitkeep).
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

# --- tests: each file has its own main() and is its own target -----------------
enable_testing()
foreach(test_name test_containers test_loader test_router test_coordinator)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE resqgrid_core)
    target_compile_definitions(${test_name} PRIVATE
        RESQGRID_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    resqgrid_warnings(${test_name})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
