cmake_minimum_required(VERSION 3.14)

project(cheetah_inekf_lcm)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
# add_compile_options(-std=c++11)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)
# add_compile_options(-O3)
# add_compile_options(-DEIGEN_NO_DEBUG)
# add_compile_options(-march=native)
#set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -DEIGEN_NO_DEBUG -march=native -Wl,--no-as-needed -DBOOST_LOG_DYN_LINK -lboost_log -lpthread") #-DBOOST_LOG_DYN_LINK -lboost_log -lpthread needed for boost::log
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
SET(CMAKE_C_COMPILER /usr/bin/gcc)

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")

SET(CMAKE_MODULE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/cmake)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS 
  roscpp
  tf
  std_msgs
  geometry_msgs
  sensor_msgs
  #inekf_msgs
  message_filters
  #apriltag_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

# Eigen 
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
message("EIGEN3_INCLUDE_DIR: " ${EIGEN3_INCLUDE_DIR})

# InEKF 
find_package(inekf REQUIRED)
include_directories(${inekf_INCLUDE_DIRS})
message("inekf_INCLUDE_DIR: " ${inekf_INCLUDE_DIRS})
link_directories(${inekf_LIBRARY_DIRS})
message("inekf_LIBRARY_DIRS: " ${inekf_LIBRARY_DIRS})

# Boost
find_package(Boost 1.58 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
message("Boost_INCLUDE_DIR: " ${Boost_INCLUDE_DIRS})

find_package(lcm REQUIRED)
if (LCM_FOUND)
    include_directories(${LCM_INCLUDE_DIRS})
    # add_subdirectory(src)
endif (LCM_FOUND)
message("LCM_INCLUDE_DIR: " ${LCM_INCLUDE_DIRS})

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES invariant-ekf-lcm
   CATKIN_DEPENDS std_msgs sensor_msgs tf geometry_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/kin
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
file(GLOB kinematics_src
    "${PROJECT_SOURCE_DIR}/src/kin/*.cpp"
)

add_library(cheetah_kinematics_lcm ${kinematics_src})

add_library(cheetah_lcm_handler "${PROJECT_SOURCE_DIR}/src/communication/lcm_handler.cpp")

add_library(cheetah_measurement "${PROJECT_SOURCE_DIR}/src/utils/measurement.cpp")
add_library(utils "${PROJECT_SOURCE_DIR}/src/utils/utils.cpp")
add_library(cheetah_state "${PROJECT_SOURCE_DIR}/src/system/cheetah_state.cpp")
add_library(cheetah_system "${PROJECT_SOURCE_DIR}/src/system/cheetah_system.cpp")
add_library(body_estimator "${PROJECT_SOURCE_DIR}/src/estimator/body_estimator.cpp")
add_library(pose_publisher_node "${PROJECT_SOURCE_DIR}/src/pose_publisher_node.cpp")

target_link_libraries(cheetah_lcm_handler cheetah_kinematics_lcm ${LCM_LIBRARY})

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

add_executable(path_publisher_node "${PROJECT_SOURCE_DIR}/src/path_publisher_node.cpp")
target_link_libraries(path_publisher_node
    ${catkin_LIBRARIES}
    ${inekf_LIBRARIES}
    ${Boost_LIBRARIES}
)

add_executable(cheetah_estimator src/cheetah_estimator.cpp )
target_link_libraries(cheetah_estimator
    ${catkin_LIBRARIES}
    ${inekf_LIBRARIES}
    ${Boost_LIBRARIES}
    cheetah_measurement
    utils
    cheetah_kinematics_lcm
    cheetah_lcm_handler
    cheetah_system
    cheetah_state
    body_estimator
    pose_publisher_node
)

# add_executable(lcm_handler src/communication/lcm_handler.cpp )
# target_link_libraries(lcm_handler
#     ${catkin_LIBRARIES}
#     ${inekf_LIBRARIES}
#     ${Boost_LIBRARIES}
#     cheetah_kinematics
# )

# add_executable(body_estimator src/estimator/body_estimator.cpp )
# target_link_libraries(body_estimator
#     ${catkin_LIBRARIES}
#     ${inekf_LIBRARIES}
#     ${Boost_LIBRARIES}
# )

# add_executable(cheetah_state src/system/cheetah_state.cpp )
# target_link_libraries(cheetah_state
#     ${catkin_LIBRARIES}
#     ${inekf_LIBRARIES}
#     ${Boost_LIBRARIES}
#     cheetah_kinematics
#     # cheetah_lcm
# )


## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_invariant-ekf-ros.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)