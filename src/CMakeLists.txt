################################################################################
# CMakeLists.txt
#
# Created on: Jan 24, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
project(src)

# Make cmake search the following Modules directory
set(CMAKE_MODULE_PATH "${${PROJECT_ROOT_NAME}_SOURCE_DIR}/Modules")

# Configure config.h and Doxyfile
configure_file(${src_SOURCE_DIR}/config.h.in ${src_BINARY_DIR}/config.h)
configure_file(${src_SOURCE_DIR}/Doxyfile.in ${src_BINARY_DIR}/Doxyfile)

# Application projects in subdirectories
add_subdirectory(Qiew)

