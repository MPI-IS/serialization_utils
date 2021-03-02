#
# set up the project
#
cmake_minimum_required(VERSION 3.10.2)

project(serialization_utils VERSION 2.0.0)

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")

#
# Dependencies
#

find_package(mpi_cmake_modules REQUIRED)
find_package(cereal REQUIRED)

# OpenCV is an optional dependencies. This package is a header library, so will
# compile fine without OpenCV installed if no package includes its OpenCV
# dependant header files
find_package(OpenCV QUIET)

# sanity check: the line above should have defined the variable OpenCV_FOUND
# (either to 1 for found or 0 for not found). If if the variable is not declared
# at all, there is something wrong.
if(NOT DEFINED OpenCV_FOUND)
  message(
    WARNING
      "serialization utils: failed to detect if OpenCV is installed or not")
endif(NOT DEFINED OpenCV_FOUND)

#
# Create the main target.
#

add_library(${PROJECT_NAME} INTERFACE)
# Add the include dependencies
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)
# External dependencies
if(OpenCV_FOUND)
  target_include_directories(${PROJECT_NAME} INTERFACE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} INTERFACE ${OpenCV_LIBRARIES})
endif()
target_link_libraries(${PROJECT_NAME} INTERFACE cereal::cereal)

# Export the target.
list(APPEND all_targets ${PROJECT_NAME})

#
# Tests.
#
include(CTest)
if(BUILD_TESTING)
  if(OpenCV_FOUND)

    # C++ unit-tests framework used.
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)
    # create the executable
    add_executable(test_serialize_cvmat test/test_serialize_cvmat.cpp)
    # link to the created librairies and its dependencies
    target_link_libraries(test_serialize_cvmat ${PROJECT_NAME} GTest::gtest)
    # declare the test as gtest
    gtest_add_tests(TARGET test_serialize_cvmat)

  endif()
endif()

#
# building documentation
#

# add_documentation()

#
# Install the package
#

# install the include directory
install(DIRECTORY include/ DESTINATION include)

# command to install the library and binaries
install(
  TARGETS ${all_targets}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

# Export this package as a cmake package.
generate_cmake_package()
