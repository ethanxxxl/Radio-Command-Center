cmake_minimum_required(VERSION 2.6)
project(Radio_Command_Center C)

set(SRC
	src/main.c
        src/parser.c
        src/extraction_game.c
	)

include_directories(./include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS  "$(CMAKE_CXX_FLAGS) -Wall")

set(CMAKE_BUILD_TYPE Debug)

add_executable(run ${SRC})

target_link_libraries(run m)
