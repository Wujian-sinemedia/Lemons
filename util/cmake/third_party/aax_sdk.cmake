set (LEMONS_AAX_AVAILABLE FALSE CACHE INTERNAL "" FORCE)

# AAX is only available on Mac and Windows...
if (NOT APPLE AND NOT WIN32)
	return()
endif()

# AAX isn't useful on iOS...
if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
	return()
endif()

# Check if the supplied AAX SDK path is valid

if (NOT LEMONS_AAX_SDK_PATH)
	return()
endif()

if (NOT IS_DIRECTORY "${LEMONS_AAX_SDK_PATH}")
	return()
endif()

# Need to know the build type
if (NOT CMAKE_BUILD_TYPE)
	message (WARNING "CMAKE_BUILD_TYPE must be specified in order to buid the AAX SDK. Skipping for this build...")
	return()
endif()

message (STATUS "Building AAX SDK...")

if (APPLE)

	find_program (XCODE_BUILD xcodebuild)

	if (NOT XCODE_BUILD)
		message (WARNING "Could not find xcodebuild - cannot build AAX SDK!")
		return()
	endif()
	
	execute_process (COMMAND "${XCODE_BUILD}" -scheme AAXLibrary_libcpp -configuration ${CMAKE_BUILD_TYPE} build 
					 WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/Libs/AAXLibrary/MacBuild"
					 RESULT_VARIABLE res)

elseif (WIN32)

	find_program (MS_BUILD msbuild)

	if (NOT MS_BUILD)
		message (WARNING "Could not find msbuild - cannot build AAX SDK!")
		return()
	endif()
	
	execute_process (COMMAND "${MS_BUILD}" AAX_SDK.sln -p:Configuration=${CMAKE_BUILD_TYPE}
					 WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/msvc"
					 RESULT_VARIABLE res COMMAND_ECHO STDOUT)
endif()

# Check if build was successful
if (res)
	message (WARNING "Error building AAX SDK -- unable to use AAX plugin format")
	return()
endif()
	
set (LEMONS_AAX_AVAILABLE TRUE CACHE INTERNAL "" FORCE)
