cmake_minimum_required(VERSION 3.10)
project(renderer LANGUAGES C)

set(SOURCE_FILES
	src/main.c
	src/render.c
	src/obj.c
	src/platforms/win32.c
)

set(HEADER_FILES
	src/render.h
	src/platform.h
	src/linear.h
	src/stretchy.h
	src/obj.h
)

set(TARGET renderer)
add_executable(${TARGET} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${TARGET} PROPERTIES C_STANDARD 99)
set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS OFF)

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
	copy_directory "${CMAKE_SOURCE_DIR}/data" "$<TARGET_FILE_DIR:${TARGET}>/data"
)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
