#  +---------------------------------------------------------------------------+
#  |                                                                           |
#  |                                                                           |
#  | Light ARIA  library                                                       |
#  |                                                                           |
#  | Authors: Mauro Bellone - http://www.maurobellone.com                      |
#  | Released under GNU GPL License.                                           |
#  +---------------------------------------------------------------------------+ */
 
# Note: This CMake file is not standalone, it defines the serial 3rd party library 
project(ArNetworking) 
 
 
set (lib_SOURCE_DIR ${PROJECT_SOURCE_DIR} ) 
set (ArNetworking_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "Path to the headers of ArNetworking lib") 

message (STATUS "${PROJECT_NAME} MESSAGE:   ---   Aria_INCLUDE_DIR    :: ${ArNetworking_INCLUDE_DIR}")

set (LIB_SOURCES ${PROJECT_SOURCE_DIR} )
set (LIB_HEADERS ${PROJECT_SOURCE_DIR}/include)
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   LIB_SOURCES    :: ${LIB_SOURCES}")
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   LIB_HEADERS    :: ${LIB_HEADERS}")
message (STATUS "${PROJECT_NAME} MESSAGE:   ---   lib_SOURCE_DIR    :: ${lib_SOURCE_DIR}")
 
 #select all headers in the include dir
FILE(GLOB LIB_HEADERS "${LIB_SOURCES}/include/*.h")   
#select all sources in the include dir
FILE(GLOB LIB_SOURCES "${LIB_SOURCES}/src/*.cpp")  


include_directories(${Aria_INCLUDE_DIR})
#add_dependencies(${PROJECT_NAME} ARIA_library) 
 
## Add serial library 
add_library(${PROJECT_NAME} SHARED ${LIB_HEADERS} ${LIB_SOURCES})  
target_link_libraries (${PROJECT_NAME}  ws2_32 winmm advapi32 Aria_library)

# set the postfix _d for debug libraries  
set_target_properties(${PROJECT_NAME}  PROPERTIES DEBUG_POSTFIX "_d") 

 
## Include headers 
include_directories(include) 
 
# allows folders for MSVC 
if (MSVC AND ENABLE_SOLUTION_FOLDERS)  
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rdParty") 
endif (MSVC AND ENABLE_SOLUTION_FOLDERS)