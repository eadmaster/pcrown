project(itemutil)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# pure C headers
set( itemutil_HEADERS
    ../eventeditor/evn.h
    ../eventeditor/texttbl.h
    ../eventeditor/util.h
)

# C sources
set( itemutil_SOURCES
	main.cpp
    ../eventeditor/evn.cpp
    ../eventeditor/texttbl.cpp
    ../eventeditor/util.c
)

# Resources
set( itemutil_RESOURCES
)

set (itemutil_LIBRARIES 
)

add_executable( itemutil
	${itemutil_HEADERS}
	${itemutil_SOURCES}
	${itemutil_RESOURCES} 
)

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../eventeditor
)

target_link_libraries( itemutil ${itemutil_LIBRARIES} )
