cmake_minimum_required(VERSION 2.8)
project(SimpleParser)

set(
	CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++17 -W -Wall -Wextra -Winline -pedantic"
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
	tests
		test.cc
)

add_executable(
	clc
		clc.cc
)

target_link_libraries(
	tests
		SimpleParser
		gtest
		pthread
)

target_link_libraries(
	clc
		SimpleParser
		boost_program_options
)

install(
	TARGETS
		clc
	DESTINATION
		bin
)

install(
	TARGETS
		SimpleParser
	DESTINATION
		lib
)
