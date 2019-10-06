# CMake version cmake@v3.10.x
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and a few useful settings. Other commands can pick up the results
project(hpc 
    VERSION 0.1.0)

# Must use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# Include CMAKE
include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)

# Build settings
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF) # Let's ensure -std=c++xx instead of -std=g++xx
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

# Find packages
# Docs only available if this is the main app
find_package(Doxygen)
# if(Doxygen_FOUND)
#     add_subdirectory(./docs)
# else()
#     message(STATUS "Doxygen not found, not building docs")
# endif()

# find_package(MPI REQUIRED) # Find OPENMPI package
# find_package(cpprestsdk REQUIRED)

# Internal includes
# include_directories(./include/)

# External includes
# include_directories(${MPI_CXX_INCLUDE_PATH}) 

# Add subdirectories - executable
add_subdirectory(./apps/)

# Add subdirectories - libraries
add_subdirectory(./libs/helloworld/)
add_subdirectory(./libs/computepi/)
add_subdirectory(./libs/random/)

# Link external libraries
# target_link_libraries(${PROJECT_NAME} MPI::MPI_C)

# Test
include(CTest)
# add_test(NAME appTest COMMAND app)

# Messages
message("CMAKE source directory = " ${CMAKE_SOURCE_DIR})
message("CMAKE binary directory = " ${CMAKE_BINARY_DIR})
message("Project source directory = " ${PROJECT_SOURCE_DIR})
message("Project binary directory = " ${PROJECT_BINARY_DIR})
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message("MPI_INCLUDE_PATH = " ${MPI_INCLUDE_PATH})