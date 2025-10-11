cmake_minimum_required(VERSION 3.31)

project(gen-dataset LANGUAGES CXX)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/sources")
set(SOURCES "${SRC_DIR}/main.cpp" "${SRC_DIR}/gen-dataset.cpp")
set(INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
find_package(fmt REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)