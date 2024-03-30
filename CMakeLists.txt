cmake_minimum_required(VERSION 3.10)
project(FFTSound)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)

add_definitions(-w)

set (CMKAE_CXX_STANDARD 14)
set (CMAKE_BUILD_TYPE Release)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (SOURCE_DIR  "${CMAKE_CURRENT_LIST_DIR}/src") 
set (INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set (GLFW_INC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw/include")
set (GLAD_SRC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/glad/src")
set (GLAD_INC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/glad/include")
set (STB_IMAGE_INC		"${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb_image")
set (GLM_INC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm")
set (IMGUI_SRC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui")
set (IMPLOT_SRC			"${CMAKE_CURRENT_LIST_DIR}/thirdparty/implot")

link_directories(
)

include_directories(
	${INCLUDE_DIR}
	${INCLUDE_DIR}/Core
	${INCLUDE_DIR}/Ui
	${GLFW_INC}
	${GLAD_INC}
	${FFTW_INCLUDE_DIRS}
	${STB_IMAGE_INC}
	${GLM_INC}
	${IMGUI_SRC}
	${IMPLOT_SRC}
) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SRC_FILES
	"${SOURCE_DIR}/*.c*"
	"${SOURCE_DIR}/Core/*.c*"
	"${SOURCE_DIR}/Ui/*.c*"
	"${GLAD_SRC}/*c*"
	"${IMGUI_SRC}/*.c*"
	"${IMPLOT_SRC}/*.c*"
)

file (GLOB_RECURSE INC_FILES
    "${INCLUDE_DIR}/*.h"
	"${IMGUI_SRC}/*.h"
	"${IMPLOT_SRC}/*.h"
	"${GLAD_INC}/*h"
)


list(APPEND EXTRA_LIBS
	GL GLU X11 glfw asound
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})

# Add the target includes for MY_PROJECT
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

file(COPY ${CMAKE_SOURCE_DIR}/configs DESTINATION ${CMAKE_BINARY_DIR})