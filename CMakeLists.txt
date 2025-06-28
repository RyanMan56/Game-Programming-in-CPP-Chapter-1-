cmake_minimum_required(VERSION 3.15)

project(Chapter1 VERSION 1.0)

# Provide a single source of truth for compiler flags
add_library(chapter_1_compiler_flags INTERFACE)
target_compile_features(chapter_1_compiler_flags INTERFACE cxx_std_20)

# Compiler conditionals
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Add compile warning flags based on the compiler used from the flags above
target_compile_options(chapter_1_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fcolor-diagnostics;-fansi-escape-codes;-Wall;-Wextra;-Wconversion;-Wsign-conversion;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_like_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Control where the static and shared libraries are built so that on Windows
# we don't need to mess with the PATH to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Gathers all cpp files in the src directory recursively
# NOTE: I will need to regenerate build files after adding any new files,
# as these won't be automatically picked up
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Configure SDL2 flags
set(SDL_SHARED OFF CACHE BOOL "" FORCE) # Build static SDL2 lib
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

# Add SDL2 from submodule
add_subdirectory(external/SDL2 EXCLUDE_FROM_ALL)

add_executable(Chapter1 src/main.cpp ${SOURCE_FILES})

# Link the compiler flags and libraries to the executable
target_link_libraries(Chapter1
  PUBLIC chapter_1_compiler_flags
  PRIVATE SDL2main SDL2-static
)

target_include_directories(Chapter1 PUBLIC
  "${PROJECT_BINARY_DIR}/src"
)

# Install the executable to the bin directory
install(TARGETS Chapter1 DESTINATION bin)
