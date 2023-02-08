cmake_minimum_required(VERSION 3.18)

#warnnig setting
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall)# -fsanitize=thread)
endif()
set(CMAKE_C_STANDARD 11)

if(WIN32)
	set(LINK_TARGET m pthread)
else()
	set(LINK_TARGET m pthread dl)# tsan)
endif()

set(FFI_TARGET_NAME fklffi)

project(${FFI_TARGET_NAME})
file(GLOB FFI_SOURCE "c/*.c")

add_subdirectory(fakeLisp)

include_directories("${PROJECT_SOURCE_DIR}/fakeLisp/include")

#fklffi.so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${FFI_TARGET_NAME}
	MODULE
	${FFI_SOURCE})
set_target_properties(${FFI_TARGET_NAME} PROPERTIES PREFIX "")

target_link_libraries(${FFI_TARGET_NAME} fkl ffi)

#copy ffi.fkl
add_custom_target(COPY_FFI_FKL ALL
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ffi.fkl ${CMAKE_BINARY_DIR})

