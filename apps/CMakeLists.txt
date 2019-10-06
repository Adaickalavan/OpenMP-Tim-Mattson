# Executable 1
add_executable(tutorial ./tutorial.cpp)
target_link_libraries(tutorial PRIVATE helloworld)
target_link_libraries(tutorial PRIVATE computepi)
target_include_directories(tutorial PRIVATE ../libs/helloworld/)
target_include_directories(tutorial PRIVATE ../libs/computepi/)

# Executable 2
add_executable(mandel ./mandel.c)
find_package(OpenMP REQUIRED) # Find OPENMP package
target_link_libraries(mandel OpenMP::OpenMP_C)

# Executable 3
add_executable(linked ./linked.c)
find_package(OpenMP REQUIRED) # Find OPENMP package
target_link_libraries(linked OpenMP::OpenMP_C)

# Executable 4
add_executable(prodCons ./prodCons.c)
find_package(OpenMP REQUIRED) # Find OPENMP package
target_link_libraries(prodCons OpenMP::OpenMP_C)

# Executable 5
add_executable(pi_mc ./pi_mc.c)
find_package(OpenMP REQUIRED) # Find OPENMP package
target_link_libraries(pi_mc OpenMP::OpenMP_C)
target_link_libraries(pi_mc random)
target_include_directories(pi_mc PRIVATE ../libs/random/)