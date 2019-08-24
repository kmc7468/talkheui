cmake_minimum_required(VERSION 3.8.0)
project(talkheui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("./include" "./extlib")
file(GLOB_RECURSE SOURCE_LIST "./src/*.cpp" "./extlib/*.c" "./extlib/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")

if(WIN32)
	link_libraries(ws2_32)
endif()

set(USE_MULTIPRECISION FALSE CACHE BOOL "")

if(${USE_MULTIPRECISION})
	add_definitions(-DTH_USE_MULTIPRECISION)
	set(BOOST_INCLUDE_DIR "" CACHE PATH "")

	if("${BOOST_INCLUDE_DIR}" STREQUAL "")
		find_package(Boost REQUIRED)
		if(${Boost_FOUND})
			include_directories(${Boost_INCLUDE_DIRS})
		endif()
	else()
		include_directories(${BOOST_INCLUDE_DIR})
	endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
install(TARGETS ${PROJECT_NAME} DESTINATION "bin")