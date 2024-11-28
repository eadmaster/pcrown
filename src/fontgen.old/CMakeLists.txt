project(fontgen)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# dependencies: allegro, libxml2

# pure C headers
set( fontgen_HEADERS
	xml.h )

# C sources
set( fontgen_SOURCES
	main.c
	xml.c )

set (FONTGEN_LIBRARIES 
    shlwapi
	${ALLEGRO_LIBRARY} 
	${LOADPNG_LIBRARY} 
	${LIBPNG_LIBRARY} 
	${LIBXML2_LIBRARY} 
    ${ICONV_LIBRARY})

add_executable( fontgen
	${fontgen_HEADERS}
	${fontgen_SOURCES}
	${fontgen_RESOURCES} )

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/images
	${ALLEGRO_INCLUDE_DIR}
	${LIBXML2_INCLUDE_DIR}
    ${ICONV_INCLUDE_DIR})

target_link_libraries( fontgen ${FONTGEN_LIBRARIES} )

add_custom_command(TARGET fontgen POST_BUILD
	COMMAND  ${CMAKE_COMMAND} -E copy ${ALLEGRO_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug
	COMMAND  ${CMAKE_COMMAND} -E copy ${LIBXML2_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug
	COMMAND  ${CMAKE_COMMAND} -E copy ${ICONV_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug
	COMMAND  ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug
    COMMENT "Copying dependencies"
)

