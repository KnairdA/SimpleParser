cmake_minimum_required(VERSION 2.8)
project(SimpleParser)

set(
	CMAKE_CXX_FLAGS
	"-std=c++14 -W -Wall -Wextra -Winline -pedantic"
)

add_library(
	SimpleParser
	SHARED
	src/nodes.cc
	src/tree.cc
	src/utils.cc
	src/parser.cc
)

add_executable(
	test
	test.cc
)

add_executable(
	clc
	clc.cc
)

target_link_libraries(
	test
	SimpleParser
	gtest
)

target_link_libraries(
	clc
	SimpleParser
)

add_custom_command(
	TARGET test
	POST_BUILD COMMAND ./test
)
