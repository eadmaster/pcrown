project(mksplash)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# dependencies: none

# pure C headers
set( mksplash_HEADERS
	common.h compress.h lzf.h lzfP.h )

# C sources
set( mksplash_SOURCES
	compress.c
	mksplash.c )

set (mksplash_LIBRARIES 
	)

add_executable( mksplash
	${mksplash_HEADERS}
	${mksplash_SOURCES}
	${mksplash_RESOURCES} )

target_link_libraries( mksplash ${mksplash_LIBRARIES} )
