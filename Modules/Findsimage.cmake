FIND_LIBRARY(SIMAGE_LIBRARY
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
) 
