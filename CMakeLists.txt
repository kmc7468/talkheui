cmake_minimum_required(VERSION 3.8.0)
project(talkheui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("./include" "./extlib")
file(GLOB_RECURSE SOURCE_LIST "./src/*.cpp" "./extlib/*.c" "./extlib/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

if(WIN32)
    link_libraries(ws2_32)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
install(TARGETS ${PROJECT_NAME} DESTINATION "bin")