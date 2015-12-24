cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

add_subdirectory("./deps/NEngine")

project(Atomix)

SET(CMAKE_CXX_WARNING_LEVEL 4)
if (DEFINED MSYS)
    add_definitions("/W4")
endif ()

if (DEFINED CMAKE_COMPILER_IS_GNUCC OR DEFINED CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-Wall -Wextra -g")
endif ()

# Version configuration
set (Atomix_VERSION_MAJOR "0")
set (Atomix_VERSION_MINOR "9")
set (Atomix_VERSION_PATCH "0")

link_directories(./lib)

file(
    GLOB_RECURSE
    sources
    src/*.cpp
    src/*.hpp
)

# Executable
add_executable(
    Atomix
    ${sources}
)

include_directories(./src)
include_directories(./deps/NEngine/include)

target_link_libraries (Atomix NEngine)

# Installation (CPack)
install(TARGETS Atomix
        RUNTIME DESTINATION .)
if (WIN32)
    install(DIRECTORY NEngine
            DESTINATION .
            FILES_MATCHING PATTERN "*.dll")
else()
    install(DIRECTORY NEngine
            DESTINATION .
            FILES_MATCHING PATTERN "*.so")
endif()

install(DIRECTORY "./data/"
        DESTINATION "./data/")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Another remake of Atomix")
set(CPACK_PACKAGE_VENDOR "LittleWhite")
set(CPACK_PACKAGE_EXECUTABLES "Atomix" "Atomix")
set(CPACK_PACKAGE_VERSION_MAJOR ${Atomix_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Atomix_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Atomix_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

include(CPack)
