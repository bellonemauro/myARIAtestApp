#  +---------------------------------------------------------------------------+
#  |                                                                           |
#  |                                                                           |
#  | Light ARIA  library                                                       |
#  |                                                                           |
#  | Authors: Mauro Bellone - http://www.maurobellone.com                      |
#  | Released under GNU GPL License.                                           |
#  +---------------------------------------------------------------------------+ */
 
# Note: This CMake file is not standalone, it defines the serial 3rd party library 
project(Aria_library) 
 
 
set (lib_SOURCE_DIR ${PROJECT_SOURCE_DIR} ) 
set (Aria_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "Path to the headers of Aria lib") 

message (STATUS "${PROJECT_NAME} MESSAGE:   ---   Aria_INCLUDE_DIR    :: ${Aria_INCLUDE_DIR}")

set (LIB_SOURCES ${PROJECT_SOURCE_DIR} )
set (LIB_HEADERS ${PROJECT_SOURCE_DIR}/include)
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   LIB_SOURCES    :: ${LIB_SOURCES}")
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   LIB_HEADERS    :: ${LIB_HEADERS}")
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   lib_SOURCE_DIR    :: ${lib_SOURCE_DIR}")
 
 #select all headers in the include dir
FILE(GLOB LIB_HEADERS "${LIB_SOURCES}/include/*.h")   
#select all sources in the include dir
FILE(GLOB LIB_SOURCES "${LIB_SOURCES}/src/*.cpp")  


if (WIN32)
   foreach (SOURCE IN LISTS LIB_SOURCES)
	  #message(STATUS "${SOURCE}")
	  string (FIND ${SOURCE} "LIN" found)
	  if (NOT found GREATER 0) 	  
   	    LIST(APPEND LIB_SOURCES_WIN "${SOURCE}" )
	  endif ()
   endforeach()
   message(STATUS "${LIB_SOURCES_WIN}")
elseif (UNIX)
   foreach (SOURCE IN LISTS LIB_SOURCES)
	  #message(STATUS "${SOURCE}")
	  string (FIND ${SOURCE} "WIN" found)
	  if (NOT found GREATER 0) 	  
   	    LIST(APPEND LIB_SOURCES_LIN "${SOURCE}" )
	  endif ()
   endforeach()
   message(STATUS "${LIB_SOURCES_LIN}")
endif (WIN32)



#SET(winmm_lib winmm.lib)
#SET(ws2_32_lib ws2_32.lib)
#SET(advapi32_lib advapi32.lib)
 
 
## Add serial library 
if (WIN32)
	add_library(${PROJECT_NAME} SHARED ${LIB_HEADERS} ${LIB_SOURCES_WIN})  
	target_link_libraries (${PROJECT_NAME}  ws2_32 winmm advapi32)
elseif(UNIX)
	add_library(${PROJECT_NAME} SHARED ${LIB_HEADERS} ${LIB_SOURCES_LIN})  #TODO: test this!
endif()


# set the postfix _d for debug libraries  
set_target_properties(${PROJECT_NAME}  PROPERTIES DEBUG_POSTFIX "_d") 

 
## Include headers 
include_directories(include) 
 
# allows folders for MSVC 
if (MSVC AND ENABLE_SOLUTION_FOLDERS)  
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rdParty") 
endif (MSVC AND ENABLE_SOLUTION_FOLDERS)