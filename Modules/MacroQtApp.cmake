################################################################################
# CMakeLists.txt
#
# Created on: Nov 24, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
macro(QTAPP_MACRO)

# set APP_NAME to enclosing directory name or Parameter 1
if(${ARGC} GREATER 0)
	set(APP_NAME "${ARGV0}")
else(${ARGC} GREATER 0)
	get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	set(APP_NAME "${DIR_NAME}")
endif(${ARGC} GREATER 0)

message(STATUS "Application: ${APP_NAME}")

project(${APP_NAME})

find_package(Qt4 4.4.3 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
find_package(Coin REQUIRED)
find_package(SoQt REQUIRED)
find_package(simage)

include(${QT_USE_FILE})
add_definitions(-DCOIN_NOT_DLL -DSOQT_NOT_DLL ${QT_DEFINITIONS})
QT4_WRAP_UI(MAINWINDOW_FILES MainWindow.ui)
QT4_WRAP_CPP(MAINWINDOWCTRL_FILES MainWindowCtrl.h)
QT4_WRAP_CPP(APPLICATION_FILES Application.h)

add_subdirectory(Resources)
if (WIN32)
	# Windows
	set(WINDOWS_RESOURCE_RC ${Windows_SOURCE_DIR}/${APP_NAME}.rc)
elseif (APPLE)
	# Macintosh
	set(MACINTOSH_RESOURCE_ICON ${Macintosh_SOURCE_DIR}/${APP_NAME}.icns)
	set(MACINTOSH_RESOURCE_PLIST ${Macintosh_SOURCE_DIR}/${APP_NAME}-Info.plist)
else (WIN32)
	# Linux / Unix
	set(LINUX_RESOURCE_ICON ${Linux_SOURCE_DIR}/${APP_NAME}.png)
	set(LINUX_RESOURCE_DESKTOP ${Linux_SOURCE_DIR}/${APP_NAME}.desktop)
endif (WIN32)

include_directories(
	${src_BINARY_DIR}
	${${APP_NAME}_BINARY_DIR}
	${COIN_INCLUDE_DIR}
	${SOQT_INCLUDE_DIR}
)

# if HDRS is not defined, set HDRS to default
if(NOT DEFINED HDRS)
	set(HDRS
		Application.h
		MainWindowCtrl.h
		main.h
	)
endif(NOT DEFINED HDRS)

# if SRCS is not defined, set SRCS to default
if(NOT DEFINED SRCS)
	set(SRCS
		Application.cpp
		MainWindowCtrl.cpp
		main.cpp
	)
endif(NOT DEFINED SRCS)

# if APP_VERSION is not defined, set APP_VERSION to VERSION
# if VERSION is not defined, set APP_VERSION to 0.1
if(NOT DEFINED APP_VERSION)
	if(NOT DEFINED VERSION)
		set(APP_VERSION 0.1)
	else(NOT DEFINED VERSION)
		set(APP_VERSION ${VERSION})
	endif(NOT DEFINED VERSION)
endif(NOT DEFINED APP_VERSION)

if (WIN32)
	# Windows
	add_executable(${APP_NAME}
		WIN32
		${HDRS}
		${SRCS}
		${APPLICATION_FILES}
		${MAINWINDOW_FILES}
		${MAINWINDOWCTRL_FILES}
		${WINDOWS_RESOURCE_RC}
	)
elseif (APPLE)
	# Macintosh
	set(MACOSX_BUNDLE_ICON_FILE ${APP_NAME}.icns)
	set(MACOSX_BUNDLE_INFO_PLIST ${MACINTOSH_RESOURCE_PLIST})
	add_executable(${APP_NAME}
		MACOSX_BUNDLE
		${HDRS}
		${SRCS}
		${APPLICATION_FILES}
		${MAINWINDOW_FILES}
		${MAINWINDOWCTRL_FILES}
	)
	execute_process(COMMAND mkdir -p ${${APP_NAME}_BINARY_DIR}/${APP_NAME}.app/Contents/Resources)
	execute_process(COMMAND cp ${MACINTOSH_RESOURCE_ICON} ${${APP_NAME}_BINARY_DIR}/${APP_NAME}.app/Contents/Resources)
	execute_process(COMMAND cp ${MACINTOSH_RESOURCE_PLIST} ${${APP_NAME}_BINARY_DIR}/${APP_NAME}.app/Contents/Info.plist)
	execute_process(COMMAND cp ${Macintosh_SOURCE_DIR}/wrl.icns ${${APP_NAME}_BINARY_DIR}/${APP_NAME}.app/Contents/Resources)
	execute_process(COMMAND cp ${Macintosh_SOURCE_DIR}/iv.icns ${${APP_NAME}_BINARY_DIR}/${APP_NAME}.app/Contents/Resources)
else (WIN32)
	# Linux / Unix
	add_executable(${APP_NAME}
		${HDRS}
		${SRCS}
		${APPLICATION_FILES}
		${MAINWINDOW_FILES}
		${MAINWINDOWCTRL_FILES}
	)
	install(FILES ${LINUX_RESOURCE_DESKTOP} DESTINATION share/applications)
	install(FILES ${LINUX_RESOURCE_ICON} DESTINATION share/pixmaps)
endif (WIN32)

target_link_libraries(${APP_NAME}
	${QT_LIBRARIES}
	${COIN_LIBRARY}
	${SOQT_LIBRARY}
)

set_target_properties("${APP_NAME}" PROPERTIES DEBUG_POSTFIX "_debug")
set_target_properties("${APP_NAME}" PROPERTIES VERSION ${APP_VERSION})

install(TARGETS "${APP_NAME}"
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	BUNDLE DESTINATION ~/Applications
)

endmacro(QTAPP_MACRO)
