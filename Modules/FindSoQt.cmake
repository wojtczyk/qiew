FIND_PATH(SOQT_INCLUDE_DIR Inventor/Qt/SoQt.h
	${CMAKE_INCLUDE_PATH}
	$ENV{COINDIR}/include
	$ENV{COIN3DDIR}/include
	/usr/local/soqt/include
	/usr/local/SoQt/include
	/usr/local/include
	/usr/include
	$ENV{ProgramFiles}/SoQt-1/include
	$ENV{ProgramFiles}/Coin3D-2/include
)

IF (NOT SOQT_INCLUDE_DIR_FOUND)
	IF (SOQT_INCLUDE_DIR)
		MESSAGE(STATUS "Looking for SoQt headers -- found " ${SOQT_INCLUDE_DIR}/Inventor/Qt/SoQt.h)
		SET(SOQT_INCLUDE_DIR_FOUND 1 CACHE INTERNAL "SoQt headers found")
	ELSE (SOQT_INCLUDE_DIR)
		MESSAGE(FATAL_ERROR 
			"-- Looking for SoQt headers -- not found\n"
			"Please install SoQt http://www.coin3d.org/ or adjust CMAKE_INCLUDE_PATH\n"
			"e.g. cmake -DCMAKE_INCLUDE_PATH=/path-to-SoQt/include ..."
		)
	ENDIF (SOQT_INCLUDE_DIR)
ENDIF (NOT SOQT_INCLUDE_DIR_FOUND)

FIND_LIBRARY(SOQT_LIBRARY
	NAMES SoQt soqt1
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/soqt/lib
	/usr/local/SoQt/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/SoQt-1/lib
	$ENV{ProgramFiles}/Coin3D-2/lib
)

IF (NOT SOQT_LIBRARY_FOUND)
	IF (SOQT_LIBRARY)
		MESSAGE(STATUS "Looking for SoQt library -- found " ${SOQT_LIBRARY})
		SET(SOQT_LIBRARY_FOUND 1 CACHE INTERNAL "SoQt library found")
	ELSE (SOQT_LIBRARY)
		MESSAGE(FATAL_ERROR 
			"-- Looking for SoQt library -- not found\n"
			"Please install SoQt http://www.coin3d.org/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-SoQt/lib ..."
		)
	ENDIF (SOQT_LIBRARY)
ENDIF (NOT SOQT_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	SOQT_INCLUDE_DIR
	SOQT_LIBRARY
) 
