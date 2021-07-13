if (NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
	set (BV_LV2_AVAILABLE TRUE CACHE INTERNAL "Availability of LV2 plugin format")
	message (STATUS "LV2 plugin format capability detected :-D")
else()
	set (BV_LV2_AVAILABLE FALSE CACHE INTERNAL "Availability of LV2 plugin format")
endif()

#

function (_bv_parse_plugin_format_list outlist inlist)

	if ("${inlist}" STREQUAL "")
		set (outlist "" PARENT_SCOPE)
		return()
	endif()

	set (valid_formats Standalone Unity VST3 AU AUv3)

	if (${BV_LV2_AVAILABLE})
		list (APPEND valid_formats LV2)
	endif()

	set (formatlist "")

	foreach (format ${inlist})
		if ("${format}" IN_LIST valid_formats)
			list (APPEND formatlist ${format})
		endif()
	endforeach()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()

#

function (_bv_create_default_format_list_for_platform outlist)

	set (formatlist Standalone)

	if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")

		list (APPEND formatlist Unity VST3)

		if (APPLE)
			list (APPEND formatlist AU)  # TODO: add AUv3 support...
		endif()

		if (${BV_LV2_AVAILABLE})
			list (APPEND formatlist LV2)
		endif()

	endif()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()

#

function (_bv_make_plugin_format_list outvar)

	if (DEFINED Formats)
		_bv_parse_plugin_format_list (formatlist "${Formats}")

		if (NOT "${formatlist}" STREQUAL "")
			message (STATUS "Format list successfully parsed from command line: ${formatlist}")
			set (${outvar} ${formatlist} PARENT_SCOPE)
			return()
		endif()
	endif()

	if (DEFINED bv_default_formats)
		_bv_parse_plugin_format_list (formatlist "${bv_default_formats}")

		if (NOT "${formatlist}" STREQUAL "")
			message (STATUS "Using project's default format list: ${formatlist}")
			set (${outvar} ${formatlist} PARENT_SCOPE)
			return()
		endif()
	endif()

	_bv_create_default_format_list_for_platform (formatlist)

    message (STATUS "Using default format list for platform: ${formatlist}")
    set (${outvar} ${formatlist} PARENT_SCOPE)

endfunction()

