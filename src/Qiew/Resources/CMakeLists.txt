################################################################################
# CMakeLists.txt
#
# Created on: Jan 25, 2011
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
project(Resources)

if (WIN32)
	add_subdirectory(Windows)
elseif (APPLE)
	add_subdirectory(Macintosh)
else (WIN32)
	add_subdirectory(Linux)
endif (WIN32)
