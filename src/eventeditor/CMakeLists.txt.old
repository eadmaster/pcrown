project(eventeditor)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# dependencies: allegro, libxml2

option(EE_USE_GUI "Use GUI instead of CLI version for event editor")
if (EE_USE_GUI)
	add_definitions(-DUSE_GUI=1)
endif()

# pure C headers
set( eventeditor_HEADERS
	evn.h main.h resource.h texttbl.h trnstext.h uihelper.h util.h
)

# C sources
set( eventeditor_SOURCES
	evn.cpp main.cpp texttbl.cpp trnstext.cpp uihelper.c util.c
)

set (eventeditor_LIBRARIES 
	${ALLEGRO_LIBRARY} )

add_executable( eventeditor
	${eventeditor_HEADERS}
	${eventeditor_SOURCES}
	${eventeditor_RESOURCES} )

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/images
	${ALLEGRO_INCLUDE_DIR} )

target_link_libraries( eventeditor ${eventeditor_LIBRARIES} )
