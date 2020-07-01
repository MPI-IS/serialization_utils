cmake_minimum_required(VERSION 3.1)
project(serialization_utils)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")

find_package(catkin REQUIRED COMPONENTS
    mpi_cmake_modules
)

# OpenCV is an optional dependencies.
# This package is a header library, so will compile
# fine without OpenCV installed if
# no package includes its OpenCV dependant
# header files
find_package(OpenCV QUIET)

# sanity check: the line above should have
# defined the variable OpenCV_FOUND (either to 1 for found
# or 0 for not found). If if the variable is not declared at all,
# there is something wrong.
if(NOT DEFINED OpenCV_FOUND)
  message(WARNING "serialization utils: failed to detect if OpenCV is installed or not")
endif(NOT DEFINED OpenCV_FOUND)

search_for_cereal_required()

catkin_package(
    INCLUDE_DIRS include
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${cereal_INCLUDE_DIRS}
)

#############
## Install ##
#############

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

###########
## Tests ##
###########

# tests are run only of OpenCV is found,
# as the tests include opencv related headers
if (OpenCV_FOUND)
    catkin_add_gtest(test_serialize_cvmat test/test_serialize_cvmat.cpp)
    target_link_libraries(test_serialize_cvmat
        ${catkin_LIBRARIES} ${cereal_LIBRARIES} ${OpenCV_LIBRARIES})
endif()
