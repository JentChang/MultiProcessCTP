
cmake_minimum_required(VERSION 3.19)

# project
project(ctp.api)

message(STATUS " ")
message(STATUS "----------------------------------------")
message(STATUS "  PROJECT:  ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

if (WIN32)
	message(STATUS "########### os is windows ###########")
	
	include_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}/../common"
		"${CMAKE_CURRENT_SOURCE_DIR}/../mmap/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/../lib/rapidjson/include" 
		"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi64_se_windows"
		#"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi_se_windows"
	)
	
	link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi64_se_windows
	#"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi_se_windows"
	)
elseif(UNIX)	
	message(STATUS "########### os is UNIX-like os's ###########")
endif()


set(ctp_api 
	Run.cpp 
	CtpMD.h CtpMD.cpp 
	CtpTD.h CtpTD.cpp 
	TDEvent.h TDEvent.cpp
	stdafx.h stdafx.cpp
	)

add_executable(ctp_api ${ctp_api})

target_link_libraries(
	ctp_api 
	mmap
	thostmduserapi_se.lib 
	thosttraderapi_se.lib
)