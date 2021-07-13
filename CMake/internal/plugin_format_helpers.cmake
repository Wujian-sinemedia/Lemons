if (NOT APPLE AND NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
	set (BV_LV2_AVAILABLE TRUE CACHE INTERNAL "Availability of LV2 plugin format")
	message (STATUS "LV2 plugin format capability detected :-D")
else()
	set (BV_LV2_AVAILABLE FALSE CACHE INTERNAL "Availability of LV2 plugin format")
endif()

#

function (_bv_parse_plugin_format_list outlist)

	set (valid_formats Standalone Unity VST3 AU AUv3)

	if (${BV_LV2_AVAILABLE})
		list (APPEND valid_formats LV2)
	endif()

	set (formatlist "")

	foreach (format ${Formats})
		if ("${format}" IN_LIST valid_formats)
			list (APPEND formatlist ${format})
		endif()
	endforeach()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()

#

function (_bv_make_plugin_format_list outvar)

	set (formatlist "")

	if (DEFINED Formats)
		_bv_parse_plugin_format_list (formatlist)
	endif()

	if (NOT "${formatlist}" STREQUAL "")
		message (STATUS "Format list successfully parsed from command line: ${formatlist}")
		set (${outvar} ${formatlist} PARENT_SCOPE)
		return()
	endif()

	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
        set (formatlist Standalone)
    elseif (APPLE)
        set (formatlist Standalone Unity VST3 AU)  # TODO: add AUv3
    elseif (${BV_LV2_AVAILABLE})
		set (formatlist Standalone Unity VST3 LV2)
    else()
        set (formatlist Standalone Unity VST3)
    endif()

    message (STATUS "Using default format list for platform: ${formatlist}")
    set (${outvar} ${formatlist} PARENT_SCOPE)

endfunction()