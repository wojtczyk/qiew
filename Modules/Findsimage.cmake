################################################################################
# CMakeLists.txt
#
# Created on: Dec 6, 2004
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
FIND_LIBRARY(SIMAGE_LIBRARY_RELEASE
	NAMES simage simage1
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/simage/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/simage-1/lib
)

FIND_LIBRARY(SIMAGE_LIBRARY_STATIC_RELEASE
	NAMES simage1s
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/simage/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/simage-1/lib
)

FIND_LIBRARY(SIMAGE_LIBRARY_DEBUG
	NAMES simage1d
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/simage/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/simage-1/lib
)

FIND_LIBRARY(SIMAGE_LIBRARY_STATIC_DEBUG
	NAMES simage1sd
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/simage/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/simage-1/lib
)


IF (SIMAGE_LIBRARY_DEBUG AND NOT SIMAGE_LIBRARY_RELEASE)
	SET(SIMAGE_LIBRARY ${SIMAGE_LIBRARY_DEBUG})
ENDIF (SIMAGE_LIBRARY_DEBUG AND NOT SIMAGE_LIBRARY_RELEASE)

IF (SIMAGE_LIBRARY_RELEASE AND NOT SIMAGE_LIBRARY_DEBUG)
	SET(SIMAGE_LIBRARY ${SIMAGE_LIBRARY_RELEASE})
ENDIF (SIMAGE_LIBRARY_RELEASE AND NOT SIMAGE_LIBRARY_DEBUG)

IF (SIMAGE_LIBRARY_DEBUG AND SIMAGE_LIBRARY_RELEASE)
	SET(SIMAGE_LIBRARY debug ${SIMAGE_LIBRARY_DEBUG} optimized ${SIMAGE_LIBRARY_RELEASE})
ENDIF (SIMAGE_LIBRARY_DEBUG AND SIMAGE_LIBRARY_RELEASE)


IF (SIMAGE_LIBRARY_STATIC_DEBUG AND NOT SIMAGE_LIBRARY_STATIC_RELEASE)
	SET(SIMAGE_LIBRARY_STATIC ${SIMAGE_LIBRARY_STATIC_DEBUG})
ENDIF (SIMAGE_LIBRARY_STATIC_DEBUG AND NOT SIMAGE_LIBRARY_STATIC_RELEASE)

IF (SIMAGE_LIBRARY_STATIC_RELEASE AND NOT SIMAGE_LIBRARY_STATIC_DEBUG)
	SET(SIMAGE_LIBRARY_STATIC ${SIMAGE_LIBRARY_STATIC_RELEASE})
ENDIF (SIMAGE_LIBRARY_STATIC_RELEASE AND NOT SIMAGE_LIBRARY_STATIC_DEBUG)

IF (SIMAGE_LIBRARY_STATIC_DEBUG AND SIMAGE_LIBRARY_STATIC_RELEASE)
	SET(SIMAGE_LIBRARY_STATIC debug ${SIMAGE_LIBRARY_STATIC_DEBUG} optimized ${SIMAGE_LIBRARY_STATIC_RELEASE})
ENDIF (SIMAGE_LIBRARY_STATIC_DEBUG AND SIMAGE_LIBRARY_STATIC_RELEASE)


IF (SIMAGE_LIBRARY_STATIC AND NOT SIMAGE_LIBRARY)
	set(SIMAGE_LIBRARY ${SIMAGE_LIBRARY_STATIC})
ENDIF (SIMAGE_LIBRARY_STATIC AND NOT SIMAGE_LIBRARY)


IF (NOT SIMAGE_LIBRARY_FOUND)
	IF (SIMAGE_LIBRARY)
		MESSAGE(STATUS "Looking for simage library -- found " ${SIMAGE_LIBRARY})
		SET(SIMAGE_LIBRARY_FOUND 1 CACHE INTERNAL "simage library found")
	ELSE (SIMAGE_LIBRARY)
		MESSAGE(STATUS
			"Looking for simage library -- not found\n"
			"Please install simage http://www.coin3d.org/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-simage/lib ...\n"
			"Warning: You won't be able to see textures until simage is installed."
		)
	ENDIF (SIMAGE_LIBRARY)
ENDIF (NOT SIMAGE_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	SIMAGE_LIBRARY
	SIMAGE_LIBRARY_DEBUG
	SIMAGE_LIBRARY_RELEASE
	SIMAGE_LIBRARY_STATIC_DEBUG
	SIMAGE_LIBRARY_STATIC_RELEASE
) 
