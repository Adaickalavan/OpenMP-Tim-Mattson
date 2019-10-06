add_library(random 
    ./random.c
    ./random.h)

# Find packages
find_package(OpenMP REQUIRED) # Find OPENMP package

# Link external libraries
target_link_libraries(random PRIVATE OpenMP::OpenMP_C)