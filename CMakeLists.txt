cmake_minimum_required(VERSION 3.1)
project(serialization_utils)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")

find_package(catkin REQUIRED)

catkin_package(
    INCLUDE_DIRS include
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
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

if (CATKIN_ENABLE_TESTING)
    find_package(OpenCV REQUIRED)
    catkin_add_gtest(test_serialize_cvmat test/test_serialize_cvmat.cpp)
    target_link_libraries(test_serialize_cvmat
        ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
endif()
