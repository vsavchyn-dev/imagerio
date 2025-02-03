cmake_minimum_required(VERSION 3.10)
project(imagerio_image_filters_applier)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name and target
set(TARGET_NAME imagerio)

# Enable compile commands export for tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set different compiler flags for Debug and Release builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
  # Debug-specific flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

  # Add DEBUG_PRINT macro for debug builds
  add_definitions(-DDEBUG_PRINT)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  # Release-specific optimization flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG -flto")
endif()

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
