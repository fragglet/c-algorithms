cmake_minimum_required(VERSION 2.6)

project(c-algorithms)

aux_source_directory(./src SRC_DIR)

add_library(calg-1.0 STATIC ${SRC_DIR})

install(TARGETS calg-1.0
		ARCHIVE	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES src/*.h
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/clag)
