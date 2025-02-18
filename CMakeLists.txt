cmake_minimum_required(VERSION 3.23)
   
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_OSX_COMPILER_ID "GNU")
	set(CMAKE_CXX_COMPILER "g++-13")
	message(STATUS "On apple computers, you need to install the last g++ compiler (g++-13)")
endif()

project(Rockable LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

######################################################################
# OPTIONS
option(ROCKABLE_USE_FT_CORR "add objectivity correction to tangent forces" OFF)
option(ROCKABLE_ENABLE_PROFILING "enable the time profiling" OFF)
option(ROCKABLE_ENABLE_BOUNDARY "enable the special boundaries like Ball or Cylinder" OFF)
option(ROCKABLE_ENABLE_SOFT_PARTICLES "enable strainning of particles" OFF)
option(ROCKABLE_ENABLE_PERIODIC "enable full periodic boundary conditions" OFF)

option(ROCKABLE_COMPILE_POSTPRO "Compile the application to run post-processing commands" OFF)
option(ROCKABLE_COMPILE_SEE "Compile the application to visualize the conf-files" ON)

option(ROCKABLE_COMPILE_SEE3 "Compile the application to edit graphically the input files" OFF)
option(ROCKABLE_COMPILE_PREPRO "Compile the preprocessings" OFF)
option(ROCKABLE_COMPILE_CONF2VTK "Convert conf files to VTK" OFF)
option(ROCKABLE_USE_TESTING "add tests" ON)

######################################################################
# compilation flags
set(CMAKE_CXX_FLAGS "-O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if(ROCKABLE_USE_FT_CORR)
	add_definitions(-DFT_CORR)
endif()

if(ROCKABLE_ENABLE_PROFILING)
	add_compile_options(-DROCKABLE_ENABLE_PROFILING)
endif()

if(ROCKABLE_ENABLE_BOUNDARY)
  add_compile_options(-DROCKABLE_ENABLE_BOUNDARY)
endif()

# get the git tag of Rockable
execute_process(
    COMMAND git describe --abbrev=0 --tags
    OUTPUT_VARIABLE ROCKABLE_GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DROCKABLE_GIT_TAG="${GIT_TAG}")

# the default mode is release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/deps)

######################################################################
##### Fetching TOOFUS
FetchContent_Declare (
	toofus
  GIT_REPOSITORY https://github.com/richefeu/toofus.git
  GIT_TAG        main
)
FetchContent_GetProperties(toofus)
if(NOT toofus_POPULATED)
  message(STATUS "Fetching toofus")
  FetchContent_Populate(toofus)
endif()

######################################################################
##### Fetching spdlog
FetchContent_Declare (
	spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.11.0
)
FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  message(STATUS "Fetching spdlog")
  FetchContent_Populate(spdlog)
	#add_subdirectory(${spdlog_SOURCE_DIR})
endif()
######################################################################
##### MATools
if(ROCKABLE_ENABLE_PROFILING)	
	FetchContent_Declare (
		matools
  	GIT_REPOSITORY https://github.com/rprat-pro/MATools
  	GIT_TAG        v1.1
	)
	FetchContent_GetProperties(matools)
	if(NOT matools_POPULATED)
  	message(STATUS "Fetching MATools")
  	FetchContent_Populate(matools)
		SET(MATOOLS_MPI OFF CACHE BOOL "Use some expat option")		
		add_subdirectory(${FETCHCONTENT_BASE_DIR}/matools-src)
	endif()
endif()
######################################################################
##### tclap
FetchContent_Declare (
	tclap
  GIT_REPOSITORY https://github.com/xguerin/tclap.git
  GIT_TAG        v1.2.5
)
FetchContent_GetProperties(tclap)
if(NOT tclap_POPULATED)
  message(STATUS "Fetching tclap")
  FetchContent_Populate(tclap)
	#add_subdirectory(${tclap_SOURCE_DIR})
endif()
######################################################################
##### nlohmann-json
FetchContent_Declare (
	nlohmann-json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.2
)
FetchContent_GetProperties(nlohmann-json)
if(NOT nlohmann-json_POPULATED)
  message(STATUS "Fetching nlohmann-json")
  FetchContent_Populate(nlohmann-json)
	#add_subdirectory(${nlohmann-json_SOURCE_DIR})
endif()
######################################################################
##### imgui
if(ROCKABLE_COMPILE_SEE3)
FetchContent_Declare (
	imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.89.5
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  message(STATUS "Fetching imgui")
  FetchContent_Populate(imgui)
endif()
endif() # end of ROCKABLE_COMPILE_SEE3


####################################################################
### Chaiscript
#FetchContent_Declare (
#	ChaiScript
#  GIT_REPOSITORY https://github.com/ChaiScript/ChaiScript.git
#  GIT_TAG        v6.1.0
#)
#FetchContent_GetProperties(ChaiScript)
#if(NOT chaiscript_POPULATED)
#  message(STATUS "Fetching ChaiScript")
#  FetchContent_Populate(ChaiScript)
#	add_subdirectory(${chaiscript_SOURCE_DIR})
#endif()

######################################################################
##### Making available all fetched things
#FetchContent_MakeAvailable(MATools)

######################################################################
##### OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

######################################################################
##### OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  message(STATUS "OPENGL_INCLUDE_DIR = " ${OPENGL_INCLUDE_DIR})
  message(STATUS "OPENGL_LIBRARIES = " ${OPENGL_LIBRARIES})
endif()

######################################################################
#### glut
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
	
  # here, we do not uses find_package(glut) so that the version installed by apple is NOT used
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GLUT REQUIRED glut>=3.4.0)
  if(GLUT_FOUND)
    message(STATUS "GLUT as been found with PkgConfig")		
		message(STATUS "GLUT_LIBRARY_DIRS = " ${GLUT_LIBRARY_DIRS})
	  message(STATUS "GLUT_INCLUDE_DIRS = " ${GLUT_INCLUDE_DIRS})
		message(STATUS "GLUT_LIBRARIES = " ${GLUT_LIBRARIES})
  endif() 

else()

  find_package(GLUT REQUIRED)
  if(GLUT_FOUND)
    message(STATUS "GLUT_INCLUDE_DIRS = " ${GLUT_INCLUDE_DIRS})
	  message(STATUS "GLUT_LIBRARIES = " ${GLUT_LIBRARIES})
  endif()
	
endif()

######################################################################
#### glfw
if(ROCKABLE_COMPILE_SEE3)
  find_package(glfw3 REQUIRED)
endif()

######################################################################
##### libRockable_core
set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(
	${CMAKE_SOURCE_DIR} 
	${toofus_SOURCE_DIR}
	${spdlog_SOURCE_DIR}/include
	${tclap_SOURCE_DIR}/include
)

file(GLOB core_object_files 
  ${CMAKE_SOURCE_DIR}/BodyForces/*.cpp
  ${CMAKE_SOURCE_DIR}/Core/*.cpp
  ${CMAKE_SOURCE_DIR}/DataExtractors/*.cpp
  ${CMAKE_SOURCE_DIR}/ForceLaws/*.cpp
  ${CMAKE_SOURCE_DIR}/PostProcessors/*.cpp
  ${CMAKE_SOURCE_DIR}/PreproCommands/*.cpp
  ${CMAKE_SOURCE_DIR}/ProcessingTools/*.cpp
)

if(ROCKABLE_ENABLE_BOUNDARY)
file(GLOB boundaries_files ${CMAKE_SOURCE_DIR}
	/Boundaries/*.cpp
)
list(APPEND core_object_files ${boundaries_files})
endif() # end of ROCKABLE_ENABLE_BOUNDARY

add_library(Rockable_core STATIC ${core_object_files})
if(ROCKABLE_ENABLE_PROFILING)
	target_link_libraries(Rockable_core MATools)
endif()

######################################################################
##### rockable executable (to run simulations from the console)
add_executable(rockable ${CMAKE_SOURCE_DIR}/Apps/run.cpp)
target_link_libraries(rockable PUBLIC Rockable_core)
install(TARGETS rockable DESTINATION ./)

######################################################################
##### postpro
add_executable(postpro ${CMAKE_SOURCE_DIR}/Apps/postpro.cpp)
target_link_libraries(postpro PUBLIC Rockable_core)

######################################################################
##### visualiser see
if(ROCKABLE_COMPILE_SEE)
add_executable(see ${CMAKE_SOURCE_DIR}/Apps/see.cpp)
target_include_directories(see PUBLIC 
	${OPENGL_INCLUDE_DIR} 
	${GLUT_INCLUDE_DIRS}
	${nlohmann-json_SOURCE_DIR}/include
)
target_link_directories(see PUBLIC ${GLUT_LIBRARY_DIRS})
if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries (see PUBLIC OpenGL::GL OpenGL::GLU ${GLUT_LIBRARIES} Rockable_core)
else()
  target_link_libraries (see PUBLIC OpenGL::GL OpenGL::GLU ${GLUT_LIBRARIES} Rockable_core)
endif()

install(TARGETS see DESTINATION ./)
endif()


######################################################################
##### interactive preprocessing (TODO see3 -> will be renamed something like 'craft' or 'look')
##### Before, the use of glfw will be replaced by glut (to avoid issues with macosx) 
if(ROCKABLE_COMPILE_SEE3)
set(imgui_object_files
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(see3_imgui STATIC ${imgui_object_files})
target_link_libraries(see3_imgui OpenGL::GL OpenGL::GLU glfw)
include_directories( 
	${OPENGL_INCLUDE_DIR} 
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
  ${nlohmann-json_SOURCE_DIR}/include
)

#if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
#  target_link_libraries (see3 PUBLIC OpenGL::GL OpenGL::GLU glfw Rockable_core)
#else()
#  target_link_libraries (see3 PUBLIC OpenGL::GL OpenGL::GLU glfw Rockable_core)
#endif()

add_executable(see3 ${CMAKE_SOURCE_DIR}/Apps/see3.cpp)
target_link_libraries(see3 PUBLIC see3_imgui Rockable_core)
#target_include_directories(see3 PUBLIC 
#	${OPENGL_INCLUDE_DIR} 
#	${imgui_SOURCE_DIR}
#	${imgui_SOURCE_DIR}/backends
# ${nlohmann-json_SOURCE_DIR}/include
#)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries (see3 PUBLIC OpenGL::GL OpenGL::GLU glfw Rockable_core)
else()
  target_link_libraries (see3 PUBLIC OpenGL::GL glfw Rockable_core)
endif()
install(TARGETS see3 DESTINATION ./)

endif() # end of ROCKABLE_COMPILE_SEE3

######################################################################
##### 
if(ROCKABLE_COMPILE_CONF2VTK)
  add_executable(conftovtk ${CMAKE_SOURCE_DIR}/Apps/conftovtk.cpp)
  target_link_libraries (conftovtk Rockable_core)
  install(TARGETS conftovtk DESTINATION ./)
endif()

######################################################################
##### Enable non regression tests
if(ROCKABLE_USE_TESTING)
  enable_testing()
	add_subdirectory(test)
endif()

######################################################################
##### Pre-processing tools
if(ROCKABLE_COMPILE_PREPRO)
	add_subdirectory(prepro)
endif()

