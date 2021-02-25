
cmake_minimum_required(VERSION 3.19)

# project
project(strategy)

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
		./template
		"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi64_se_windows"
	)
	link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/ctpapi/6.3.15_20190220_tradeapi64_se_windows
	)
	
elseif(UNIX)	
	message(STATUS "########### os is UNIX-like os's ###########")
endif()


set(strategy
	Strategy_test.cpp
	./template/StrategyTemplate.h ./template/StrategyTemplate.cpp
	./template/StrategyEvent.h ./template/StrategyEvent.cpp
	)

add_executable(strategy ${strategy})

target_link_libraries(
	strategy
	mmap
	thostmduserapi_se.lib 
	thosttraderapi_se.lib
)
