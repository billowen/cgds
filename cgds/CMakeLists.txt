cmake_minimum_required (VERSION 3.0)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
    ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
find_package(Qt5Gui)
find_package(Qt5Widgets)

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
    techfile.h
    canvas.h
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
    techfile.cpp
    canvas.cpp
	)
add_library (libgds STATIC ${INCS} ${SRCS})
add_executable(viewer main.cpp)



target_include_directories(libgds PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(libgds Qt5::Gui)
target_link_libraries(viewer Qt5::Widgets)






