add_library(helloworld 
    ./helloworld.cpp
    ./helloworld.h)

# Find packages
find_package(OpenMP REQUIRED) # Find OPENMP package

# Link external libraries
target_link_libraries(helloworld OpenMP::OpenMP_CXX)