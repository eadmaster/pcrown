project(pcrown)
cmake_minimum_required(VERSION 3.0)

set(PCROWN_PACKAGE pcrown)
set(PCROWN_VERSION_MAJOR 0)
set(PCROWN_VERSION_MINOR 1)
set(PCROWN_VERSION_PATCH 0)
set(PCROWN_VERSION "${PCROWN_VERSION_MAJOR}.${PCROWN_VERSION_MINOR}.${PCROWN_VERSION_PATCH}")

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_VERSION_MAJOR ${PCROWN_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PCROWN_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PCROWN_VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR "Princess Crown team")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "pcrown-${PCROWN_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "pcrown-${PCROWN_VERSION}")

set(SATURN_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/Platform/SegaSaturn")

add_subdirectory(src)
