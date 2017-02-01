################################################################################
# CMakeLists.txt
#
# Created on: Jan 24, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
cmake_minimum_required(VERSION 2.8)

# Set the Project root name
get_filename_component(PROJECT_ROOT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_ROOT_NAME Qiew)
# Set the Project name
project(${PROJECT_ROOT_NAME} CXX C)

# Set Package Base Parameters
set(PACKAGE ${PROJECT_NAME})
set(VERSION 0.5.3)

set(PACKAGE_NAME ${PACKAGE})
set(PACKAGE_VERSION ${VERSION})
set(PACKAGE_BUGREPORT "Dr. Martin Wojtczyk")
set(PACKAGE_STRING "${PROJECT_NAME} ${PACKAGE_VERSION} - A VRML/Inventor Viewer.")
set(PACKAGE_TARNAME ${PACKAGE})

add_subdirectory(src)

include(CPack)
