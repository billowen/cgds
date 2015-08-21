cmake_minimum_required (VERSION 3.0)

project (cgds)
SET (INCS 
	boundary.h
	gdsio.h
	tags.h
	elements.h
	path.h
	structures.h
	sref.h
	aref.h
	node.h
	text.h
	library.h
)
SET (SRCS
	boundary.cpp
	gdsio.cpp
	elements.cpp
	path.cpp
	structures.cpp
	sref.cpp
	aref.cpp
	node.cpp
	text.cpp
	library.cpp
	)
add_library (libgds ${INCS} ${SRCS})



target_include_directories(libgds PUBLIC ${CMAKE_SOURCE_DIR})







