cmake_minimum_required(VERSION 3.10)
project(imagerio_image_filters_applier)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name and target
set(TARGET_NAME imagerio)

# Enable compile commands export for tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optimization and warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O0")

# Find required packages
find_package(OpenMP REQUIRED)
find_package(TBB)

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
    "src/*/*.cpp"
    "src/*/*/*.cpp"
)

# Create executable
add_executable(${TARGET_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${TARGET_NAME}
    PRIVATE
    include
    ${MPI_INCLUDE_PATH}
    ${TBB_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

# Link libraries
target_link_libraries(${TARGET_NAME}
    PRIVATE
    OpenMP::OpenMP_CXX
    TBB::tbb
)

# Add OpenMP compile options
# target_compile_options(${TARGET_NAME}
#     PRIVATE
#     ${OpenMP_CXX_FLAGS}
# )
