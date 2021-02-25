cmake_minimum_required(VERSION 3.14)
PROJECT(mmap)

IF (USE_PAGED_SERVICE)
	ADD_DEFINITIONS(-DUSE_PAGED_SERVICE)
	MESSAGE(STATUS "${PROJECT_NAME} USE_PAGED_SERVICE")
ENDIF (USE_PAGED_SERVICE)


MESSAGE(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")



IF (WIN32)
	MESSAGE(STATUS "###########################OS is windows###########################")
	include_directories(
					../common
					./include
					"${CMAKE_CURRENT_SOURCE_DIR}/../lib/rapidjson/include"
					)

ELSEIF (UNIX)	
	MESSAGE(STATUS "###########################OS is UNIX-like OS's.###########################")
	
ENDIF ()


SET(MMAP_SOURCE_FILES
	../common/SystemDir.cpp
	../common/SystemDir.h
	../common/Timer.cpp
	../common/Timer.h

	./include/constants.h
	./include/Frame.hpp
	./include/FrameHeader.h
	./include/Hash.hpp
	./include/IJournalVisitor.h
	./include/IPageProvider.h
	./include/Journal.h
	./include/JournalHandler.h
	./include/JournalReader.h
	./include/JournalWriter.h
	./include/Page.h
	./include/PageHeader.h
	./include/PageProvider.h
	./include/PageUtil.h
	./include/YJJ_DECLARE.h
	
	Journal.cpp
	JournalHandler.cpp
	JournalReader.cpp
	JournalWriter.cpp
	Page.cpp
	PageProvider.cpp
	PageUtil.cpp
)


ADD_LIBRARY(${PROJECT_NAME} ${MMAP_SOURCE_FILES})
ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${MMAP_SOURCE_FILES})

IF (WIN32)
    MESSAGE(STATUS "###########################OS is windows###########################")

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
    SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES COMPILE_FLAGS "/MP")

ELSEIF (UNIX)
    MESSAGE(STATUS "###########################OS is UNIX-like OS's.###########################")

ENDIF ()
