cmake_minimum_required(VERSION 3.10)
project (Aria_interfaceApplication)


option(ENABLE_verbose "Verbose cmake cofiguration " "OFF")
if ( ENABLE_verbose )
	message(WARNING " Verbose configuration active ! You may get a lot of messages now ... " )
endif ( ENABLE_verbose )

#cmake tweaks
if(COMMAND cmake_policy) #return true if the COMMAND cmake_policy exist - this allows compatibility with older versions
	# We don't want to mix relative and absolute paths in linker lib lists.
	cmake_policy(SET CMP0003 NEW) # Required by CMake 2.7+
	
	#Show error on non-existent dependency in add_dependencies - see https://cmake.org/cmake/help/v3.0/policy/CMP0046.html
	cmake_policy(SET CMP0046 NEW) 
	
	# avoid link warnings - see http://www.cmake.org/cmake/help/v3.0/policy/CMP0015.html
	cmake_policy(SET CMP0015 NEW)
	
	#Ignore COMPILE_DEFINITIONS_<Config> properties - see http://www.cmake.org/cmake/help/v3.0/policy/CMP0043.html
	cmake_policy(SET CMP0043 NEW) 
	
	#The target in the TARGET signature of add_custom_command() must exist - see https://cmake.org/cmake/help/v3.0/policy/CMP0040.html
	cmake_policy(SET CMP0040 NEW)
	
	#Only interpret if() arguments as variables or keywords when unquoted. - see https://cmake.org/cmake/help/v3.1/policy/CMP0054.html
	cmake_policy(SET CMP0054 NEW)
	
	# Allow using the LOCATION target property. https://cmake.org/cmake/help/v3.1/policy/CMP0026.html?highlight=cmp0026
	#if(NOT "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 3.0)
	cmake_policy(SET CMP0026 NEW)
	#endif(NOT "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 3.0)
		
endif(COMMAND cmake_policy)


# Our own CMake configurations
include(${PROJECT_SOURCE_DIR}/CMakeModules/script_init_cmake.cmake REQUIRED)		

include(${CMAKE_MODULE_PATH}/UtilsMacros.cmake REQUIRED)		        # CMake macros


#  Add option enable visual studio to create folders
option(ENABLE_SOLUTION_FOLDERS "Enable folders for MSVC - ON by default if MSVC - OFF otherwise" ON)
if (MSVC)
  set (ENABLE_SOLUTION_FOLDERS 1)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else (MSVC)
  set (ENABLE_SOLUTION_FOLDERS 0)
endif (MSVC) 



# -------------------- 3rParty --------------------------------
OPTION(ENABLE_BUILD_3rdParty "Option for building 3rdParty libraries - ON by default" ON)
if (ENABLE_BUILD_3rdParty)
  add_subdirectory(3rdParty)
endif(ENABLE_BUILD_3rdParty)



# -------------------- apps --------------------------------
OPTION(ENABLE_BUILD_apps "Option for building apps - ON by default" ON)
if (ENABLE_BUILD_apps)
	add_subdirectory(apps)
endif(ENABLE_BUILD_apps)




