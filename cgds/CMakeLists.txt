cmake_minimum_required (VERSION 3.0)
find_package(Qt5Gui REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
    graphicsitems.h
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
    graphicsitems.cpp
	)
add_library (libgds ${INCS} ${SRCS})



target_include_directories(libgds PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(libgds Qt5::Gui)






