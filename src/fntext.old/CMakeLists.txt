project(fntext)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# dependencies: allegro, libxml2

# pure C headers
set( fntext_HEADERS
	 )

# C sources
set( fntext_SOURCES
	main.c )

set (fntext_LIBRARIES 
	${ALLEGRO_LIBRARY} )

add_executable( fntext
	${fntext_HEADERS}
	${fntext_SOURCES}
	${fntext_RESOURCES} )

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/images
	${ALLEGRO_INCLUDE_DIR} )

target_link_libraries( fntext ${fntext_LIBRARIES} )
