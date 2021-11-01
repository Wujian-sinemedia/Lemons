# AAX is only available on MacOS and Windows...
if (IOS OR NOT (APPLE OR WIN32))
	return()
endif()

# Check if the supplied AAX SDK path is valid
if (NOT LEMONS_AAX_SDK_PATH)
	return()
endif()

if (NOT IS_DIRECTORY "${LEMONS_AAX_SDK_PATH}")
	return()
endif()


if (APPLE)

	find_program (XCODE_BUILD xcodebuild)

	if (XCODE_BUILD)
		
		add_custom_target (AAXSDK
					   	   COMMAND "${XCODE_BUILD}" "-scheme" "AAXLibrary_libcpp" "-configuration" "$<COMMAND_CONFIG:$<CONFIG>>" "build"
					   	   COMMAND_EXPAND_LISTS VERBATIM
					   	   WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/Libs/AAXLibrary/MacBuild"
					   	   COMMENT "Building AAX SDK..."
					   	   COMMAND_ECHO STDOUT)
	endif()

elseif (WIN32)

	find_program (MS_BUILD msbuild)

	if (MS_BUILD)
		
		add_custom_target (AAXSDK
					   	   COMMAND "${MS_BUILD}" "AAX_SDK.sln" "-p:Configuration=$<COMMAND_CONFIG:$<CONFIG>>"
					   	   COMMAND_EXPAND_LISTS VERBATIM
					   	   WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/msvc"
					   	   COMMENT "Building AAX SDK..."
					   	   COMMAND_ECHO STDOUT)
	endif()

endif()


if (TARGET AAXSDK)
	#set_target_properties (AAXSDK PROPERTIES OSX_ARCHITECTURES x86_64)
endif()
