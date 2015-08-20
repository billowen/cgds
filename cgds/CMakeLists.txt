cmake_minimum_required (VERSION 3.0)

project (cgds)
SET (INCS 
	boundary.h
	gdsio.h
	tags.h
	elements.h
	path.h
	structures.h
)
SET (SRCS
	boundary.cpp
	gdsio.cpp
	elements.cpp
	path.cpp
	structures.cpp
	)
add_library (libgds ${INCS} ${SRCS})



target_include_directories(libgds PUBLIC ${CMAKE_SOURCE_DIR})







