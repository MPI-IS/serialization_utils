#
# set up the project
#
cmake_minimum_required(VERSION 3.10.2)

project(serialization_utils)

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")

#
# Dependencies
#
# depend on ament macros
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

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
ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
list(APPEND all_targets ${PROJECT_NAME})
list(APPEND all_target_exports export_${PROJECT_NAME})

#
# Tests.
#
if(BUILD_TESTING)
  # tests are run only of OpenCV is found, as the tests include opencv related
  # headers
  if(OpenCV_FOUND)
    # find gtest and load the ament macro to use it.
    find_package(ament_cmake_gtest)
    # Add the test
    ament_add_gtest(test_serialize_cvmat test/test_serialize_cvmat.cpp)
    # Link the dependecies to it.
    target_link_libraries(test_serialize_cvmat ${PROJECT_NAME})
  endif()
endif()

#
# building documentation
#

# add_documentation()

#
# Install the package
#
install(DIRECTORY include/ DESTINATION include)

install(
  TARGETS ${all_targets}
  EXPORT ${all_target_exports}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

#
# Export the package as ament
#
ament_package()
