# Minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(longtime VERSION 1.0 LANGUAGES CXX)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

# Set the C++ standard  
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# This assumes the SDL source is available in vendored/SDL  
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)  

# Add the source files  
# Replace "src" with your source directory and list your source files  
set(SOURCES
 src/main.cpp
)  

# Add the executable  
add_executable(${PROJECT_NAME} ${SOURCES})  

# Add headers from include folder  
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link with the SDL3 library  
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Link with OpenGL  
find_package(OpenGL REQUIRED)  
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# Enable warnings  
if (MSVC)  
 # Use /external:W0 to disable warnings from external headers
 target_compile_options(${PROJECT_NAME} PRIVATE /W4 /external:W0 /external:anglebrackets)  
else()  
 target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)  
endif()
