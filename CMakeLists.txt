cmake_minimum_required (VERSION 2.8)
project (SDL_gfx)
set (SDL_gfx_VERSION_MAJOR 3)
set (SDL_gfx_VERSION_MINOR 0)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package (SDL2 REQUIRED)

include_directories (
	"${SDL2_INCLUDE_DIR}"
	)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.c)
file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/*.h)

add_library (SDL_gfx SHARED
	${SOURCE_FILES}
	${INCLUDE_FILES}
	)

target_link_libraries (SDL_gfx
	${SDL2_LIBRARY}
	)

install (TARGETS SDL_gfx DESTINATION lib)
install (FILES ${INCLUDE_FILES} DESTINATION include/SDL)
