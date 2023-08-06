cmake_minimum_required(VERSION 3.22)

project(Platformer2D LANGUAGES C CXX)

set(EXTERNAL_DIR "$ENV{External}")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_compile_definitions("$<$<CONFIG:Debug>:DEBUG>")

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
	add_compile_options(-ffast-math)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/fp:fast)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/glad external/glad)
add_subdirectory(${EXTERNAL_DIR}/glfw external/glfw)
add_subdirectory(${EXTERNAL_DIR}/glm external/glm)

target_link_libraries(${PROJECT_NAME} glfw glad glm)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${EXTERNAL_DIR}/rapidxml"
	"${EXTERNAL_DIR}/stb"
	"${CMAKE_SOURCE_DIR}/include"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
	