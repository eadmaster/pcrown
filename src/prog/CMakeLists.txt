CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include(CMakeForceCompiler)
include(ExternalProject)

SET(CMAKE_SYSTEM_NAME SegaSaturn)

if (WIN32)
	set(SH_ELF_PATHS 
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\KPIT\\GNUSH v12.02-ELF;Path]/sh-elf/bin"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\KPIT Cummins\\GNUSH v10.02-ELF;Path]/sh-elf/bin")
else (WIN32)	        
	set(SH_ELF_PATHS "")
endif (WIN32)

find_program(SH_ELF_GCC
	NAMES sh-elf-gcc
	PATHS
		${SH_ELF_PATHS})

find_program(SH_ELF_GPP
	NAMES sh-elf-g++
	PATHS
		${SH_ELF_PATHS})

# Specify the cross compiler.
CMAKE_FORCE_C_COMPILER(${SH_ELF_GCC} GNU)
CMAKE_FORCE_CXX_COMPILER(${SH_ELF_GPP} GNU)
SET(CMAKE_FIND_ROOT_PATH /usr/local/share/sh-elf/bin)

enable_language(ASM)
set(prog_SOURCES
	0-bin.s
	new-asm.s
	#old-asm.s
	overlay.s
	text.c)

set(prog_LIBRARIES
	)

project(prog)

set(CMAKE_CONFIGURATION_TYPES Release)

option(PROG_WANT_DEBUG "Enable Debug" OFF)
if (PROG_WANT_DEBUG)
	add_definitions(-DENABLE_DEBUG=1)
endif()

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

enable_language(ASM)
add_executable(${PROJECT_NAME} ${prog_SOURCES})

target_link_libraries(${PROJECT_NAME} ${prog_LIBRARIES} )
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall -m2 -Os")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-nostartfiles -Wl,--script,${CMAKE_SOURCE_DIR}/sl.lk")

#add_custom_command(TARGET ${PROJECT_NAME}
#	POST_BUILD
#	DEPENDS ${PROJECT_NAME}
#	COMMAND ${CMAKE_STRIP} --remove-section=.tors -o 0-stripped.$(KPITOUTPUT) 0.$(KPITOUTPUT)
#    COMMENT "Generating stripped 0-stripped.elf" )

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	DEPENDS ${PROJECT_NAME}
	COMMAND ${CMAKE_STRIP} ${PROJECT_NAME}.elf --strip-all --output-target=binary -o 0.bin
    COMMENT "Generating stripped 0.bin" )

install(FILES ${CMAKE_BINARY_DIR}/0.BIN DESTINATION ".")

project(stg_dat2)

set(CMAKE_CONFIGURATION_TYPES Release)

option(PROG_WANT_DEBUG "Enable Debug" OFF)
if (PROG_WANT_DEBUG)
	add_definitions(-DENABLE_DEBUG=1)
endif()

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

enable_language(ASM)
add_executable(${PROJECT_NAME} ${prog_SOURCES})

target_link_libraries(${PROJECT_NAME} ${prog_LIBRARIES} )
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall -m2 -Os")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-nostartfiles -Wl,--script -Wl,${CMAKE_SOURCE_DIR}/overlay.lk")

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	DEPENDS ${PROJECT_NAME}
	COMMAND ${CMAKE_STRIP} ${PROJECT_NAME}.elf --remove-section=.text --remove-section=bss --output-target=binary -o STG_DAT2.PRG
    COMMENT "Generating stripped STG_DAT2.PRG" )

install(FILES ${CMAKE_BINARY_DIR}/STG_DAT2.PRG DESTINATION ".")

#source_group("Source Files" REGULAR_EXPRESSION .*\\.s$)
