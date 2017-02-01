FIND_PATH(COIN_INCLUDE_DIR Inventor/So.h
	${CMAKE_INCLUDE_PATH}
	$ENV{COINDIR}/include
	$ENV{COIN3DDIR}/include
	/usr/local/coin/include
	/usr/local/Coin/include
	/usr/local/include
	/usr/include
	$ENV{ProgramFiles}/Coin3D-2/include
)

IF (NOT COIN_INCLUDE_DIR_FOUND)
	IF (COIN_INCLUDE_DIR)
		MESSAGE(STATUS "Looking for Coin3D headers -- found " ${COIN_INCLUDE_DIR}/Inventor/So.h)
		SET(COIN_INCLUDE_DIR_FOUND 1 CACHE INTERNAL "Coin3D headers found")
	ELSE (COIN_INCLUDE_DIR)
		MESSAGE(FATAL_ERROR 
			"-- Looking for Coin3D headers -- not found\n"
			"Please install Coin3D http://www.coin3d.org/ or adjust CMAKE_INCLUDE_PATH\n"
			"e.g. cmake -DCMAKE_INCLUDE_PATH=/path-to-Coin/include ..."
		)
	ENDIF (COIN_INCLUDE_DIR)
ENDIF (NOT COIN_INCLUDE_DIR_FOUND)

FIND_LIBRARY(COIN_LIBRARY
	NAMES Coin coin2
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{COINDIR}/lib
	$ENV{COIN3DDIR}/lib
	/usr/local/coin/lib
	/usr/local/lib
	/usr/lib
	$ENV{ProgramFiles}/Coin3D-2/lib
)

IF (NOT COIN_LIBRARY_FOUND)
	IF (COIN_LIBRARY)
		MESSAGE(STATUS "Looking for Coin3D library -- found " ${COIN_LIBRARY})
		SET(COIN_LIBRARY_FOUND 1 CACHE INTERNAL "Coin3D library found")
	ELSE (COIN_LIBRARY)
		MESSAGE(FATAL_ERROR 
			"-- Looking for Coin3D library -- not found\n"
			"Please install Coin3D http://www.coin3d.org/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-Coin/lib ..."
		)
	ENDIF (COIN_LIBRARY)
ENDIF (NOT COIN_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	COIN_INCLUDE_DIR
	COIN_LIBRARY
) 
