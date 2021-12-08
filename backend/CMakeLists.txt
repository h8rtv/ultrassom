cmake_minimum_required(VERSION 3.10)
project(Ultrassom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

add_definitions(
  ## SQLite database file
  -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"
  ## Path to database migration scripts
  -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
  ## Path to model matrix
  -DMODEL_MATRIX="${CMAKE_CURRENT_SOURCE_DIR}/data/H-1.csv"
  ## Number of parallel tasks
  -DPARALLEL_TASKS=4
  ## Directory to persist the images
  -DOUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/out/"
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ultrassom src/main.cpp
                         src/Algorithm/CGNRSolver.cpp
                         src/Algorithm/CGNESolver.cpp)

target_include_directories(ultrassom PUBLIC src)

target_link_libraries(ultrassom ${CONAN_LIBS})