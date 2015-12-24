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

# Finding libraries
find_package (SDL)
if (SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  target_link_libraries (Atomix ${SDL_LIBRARY})
  # MESSAGE("  SDL_INCLUDE_DIR='${SDL_INCLUDE_DIR}'")
else()
  message(FATAL_ERROR "SDL not found, please install it (devel packages)")
endif (SDL_FOUND)

find_package (SDL_image)
if (SDLIMAGE_FOUND)
  include_directories(${SDLIMAGE_INCLUDE_DIR})
  target_link_libraries (Atomix ${SDLIMAGE_LIBRARY})
  # MESSAGE("  SDLIMAGE_INCLUDE_DIR='${SDLIMAGE_INCLUDE_DIR}'")
else()
  message(FATAL_ERROR "SDL Image not found, please install it (devel packages)")
endif (SDLIMAGE_FOUND)

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
