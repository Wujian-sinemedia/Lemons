set (LEMONS_AAX_AVAILABLE FALSE CACHE INTERNAL "" FORCE)

if (NOT LEMONS_AAX_SDK_PATH)
	return()
endif()

if (NOT IS_DIRECTORY "${LEMONS_AAX_SDK_PATH}")
	return()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
	return()
endif()

if (NOT CMAKE_BUILD_TYPE)
	message (WARNING "CMAKE_BUILD_TYPE must be specified in order to buid the AAX SDK. Skipping for this build...")
	return()
endif()

if (NOT ( (APPLE AND ${CMAKE_GENERATOR} MATCHES "Xcode") OR (WIN32 AND ${CMAKE_GENERATOR} MATCHES "Visual Studio") ) )
	return()
endif()

message (STATUS "Building AAX SDK...")

if (APPLE)
	
	execute_process (COMMAND xcodebuild -scheme AAXLibrary_libcpp -configuration ${CMAKE_BUILD_TYPE} build 
					 WORKING_DIRECTORY ${LEMONS_AAX_SDK_PATH}/Libs/AAXLibrary/MacBuild
					 RESULT_VARIABLE res COMMAND_ECHO STDOUT)
elseif (WIN32)
	
	execute_process (COMMAND msbuild AAX_SDK.sln -p:Configuration=${CMAKE_BUILD_TYPE}
					 WORKING_DIRECTORY ${LEMONS_AAX_SDK_PATH}/msvc
					 RESULT_VARIABLE res COMMAND_ECHO STDOUT)
endif()

if (res)
	message (WARNING "Error building AAX SDK -- unable to use AAX plugin format")
	return()
endif()
	
set (LEMONS_AAX_AVAILABLE TRUE CACHE INTERNAL "" FORCE)