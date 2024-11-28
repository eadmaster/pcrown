project(satcode_insert)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# dependencies: none

# pure C headers
set( satcode_insert_HEADERS
    ../eventeditor/util.h
	)

# C sources
set( satcode_insert_SOURCES
	main.c
    ../eventeditor/util.c
	)

set (satcode_insert_LIBRARIES 
	)

add_executable( satcode_insert
	${satcode_insert_HEADERS}
	${satcode_insert_SOURCES}
	)

target_link_libraries( satcode_insert ${satcode_insert_LIBRARIES} )
