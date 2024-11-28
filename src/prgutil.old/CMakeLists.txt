project(prgutil)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# pure C headers
set( prgutil_HEADERS
)

# C sources
set( prgutil_SOURCES
	main.cpp
)

set (prgutil_LIBRARIES 
)

add_executable( prgutil
	${prgutil_HEADERS}
	${prgutil_SOURCES}
)

target_link_libraries( prgutil ${prgutil_LIBRARIES} )
