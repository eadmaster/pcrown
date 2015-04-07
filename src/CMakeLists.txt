project(pcrown)
cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
include(CPack)

# Allegro
find_path(ALLEGRO_INCLUDE_DIR
          NAMES allegro.h
          PATHS ${ALLEGRO_ROOT_PATH}/include
          REQUIRED )

if( ALLEGRO_INCLUDE_DIR )
	find_library(ALLEGRO_LIBRARY
          	NAMES allegro-4.4.2-monolith-md-debug.lib
          	PATHS ${ALLEGRO_INCLUDE_DIR}/../lib
          	REQUIRED )

    find_program(ALLEGRO_DLL
          	NAMES allegro-4.4.2-monolith-md-debug.dll
          	PATHS ${ALLEGRO_INCLUDE_DIR}/../bin
          	REQUIRED )

	find_library(LOADPNG_LIBRARY
          	NAMES loadpng-1.5-md-debug.lib
          	PATHS ${ALLEGRO_INCLUDE_DIR}/../lib
          	REQUIRED )

	find_library(LIBPNG_LIBRARY
        	NAMES libpng-1.4.0-static-md-debug.lib
          	PATHS ${ALLEGRO_INCLUDE_DIR}/../lib
          	REQUIRED )
endif()

# libxml2
find_path(LIBXML2_INCLUDE_DIR
          NAMES libxml/xmlreader.h
          PATHS ${LIBXML2_ROOT_PATH}/include
          REQUIRED )

if( LIBXML2_INCLUDE_DIR )
    find_program(LIBXML2_DLL
          	NAMES libxml2.dll
          	PATHS ${LIBXML2_INCLUDE_DIR}/../bin
          	REQUIRED )
	find_library(LIBXML2_LIBRARY
          	NAMES libxml2.lib
          	PATHS ${LIBXML2_INCLUDE_DIR}/../lib
          	REQUIRED )
endif()

# iconv
find_path(ICONV_INCLUDE_DIR
          NAMES iconv.h
          PATHS ${ICONV_ROOT_PATH}/include
          REQUIRED )

if ( ICONV_INCLUDE_DIR )
    find_program(ICONV_DLL
          	NAMES iconv.dll
          	PATHS ${ICONV_INCLUDE_DIR}/../bin
          	REQUIRED )
	find_library(ICONV_LIBRARY
	        NAMES iconv.lib
          	PATHS ${ICONV_INCLUDE_DIR}/../lib
          	REQUIRED )
endif()

# zlib
find_path(ZLIB_INCLUDE_DIR
          NAMES zlib.h
          PATHS ${ZLIB_ROOT_PATH}/include
          REQUIRED )

if ( ZLIB_INCLUDE_DIR )
    find_program(ZLIB_DLL
          	NAMES zlib1.dll
          	PATHS ${ZLIB_INCLUDE_DIR}/../bin
          	REQUIRED )
	find_library(ZLIB_LIBRARY
	        NAMES zlib.lib
          	PATHS ${ZLIB_INCLUDE_DIR}/../lib
          	REQUIRED )
endif()

# External proprietary tools
find_program(BMFONTGEN
          NAMES BMFontGen
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/buildcd
          REQUIRED )

find_program(ISOTOOLS
          NAMES isotools
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/buildcd
          REQUIRED )

find_program(VCDBUILD
          NAMES vcdbuild
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/buildcd
          REQUIRED )

find_program(XDELTA
          NAMES xdelta
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/buildcd
          REQUIRED )

mark_as_advanced(FORCE BMFONTGEN)
mark_as_advanced(FORCE ISOTOOLS)
mark_as_advanced(FORCE VCDBUILD)
mark_as_advanced(FORCE XDELTA)

# CD images
find_file(CDIMAGE
          NAMES PRINCESS_CROWN.BIN
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cd
          REQUIRED )

find_file(CDIMAGE_CUE
          NAMES PRINCESS_CROWN.CUE
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cd
          REQUIRED )

find_file(CDIMAGE_SATAKORE
          NAMES PRINCESS_CROWN_SATAKORE.BIN
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cd
          )

find_file(CDIMAGE_CUE_SATAKORE
          NAMES PRINCESS_CROWN_SATAKORE.CUE
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cd
          )

set(TRANS_LANG "eng" CACHE STRING "Prefix of language to translate to")

set(SCRIPT_TRANS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/script/${TRANS_LANG})
set(SCRIPT_ORIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/script/jap)

option(OLD_SH2_PATCH "Enable Old SH2 code Patching" ON)

if (OLD_SH2_PATCH)
    ExternalProject_Add(satcode
    	DOWNLOAD_COMMAND ""
    	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/satcode
    	CMAKE_GENERATOR "Unix Makefiles"
    	CMAKE_ARGS ${SATURN_CMAKE_ARGS}
    	INSTALL_COMMAND ""
    	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/satcode)
else()
	# Build Saturn SH2 code
	set(SATURN_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/Platform/SegaSaturn")
	ExternalProject_Add(prog
		DOWNLOAD_COMMAND ""
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prog
		CMAKE_GENERATOR "Unix Makefiles"
		CMAKE_ARGS ${SATURN_CMAKE_ARGS}
		INSTALL_COMMAND ""
		BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/prog)
endif()

set(FONTGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fontgen)

add_subdirectory(buildcd)
add_subdirectory(eventeditor)
add_subdirectory(fntext)
add_subdirectory(fontgen)
add_subdirectory(itemsutil)
add_subdirectory(mksplash)
add_subdirectory(prgutil)
if (OLD_SH2_PATCH)
    add_subdirectory(satcode-insert)
endif()

install(FILES readme.txt ${XDELTA} DESTINATION ".")
