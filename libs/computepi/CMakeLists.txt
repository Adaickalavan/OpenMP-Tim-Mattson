add_library(computepi 
    ./computepi.cpp 
    ./computepi.h)

# Find packages
find_package(OpenMP REQUIRED) # Find OPENMP package

# Link external libraries
target_link_libraries(computepi OpenMP::OpenMP_CXX)