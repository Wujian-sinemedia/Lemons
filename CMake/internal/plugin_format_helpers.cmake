function (_bv_parse_plugin_format_list outlist)

	set (formatlist "")

	set (valid_formats Standalone Unity VST3 AU AUv3 LV2)

	foreach (format ${Formats})
		if ("${format}" IN_LIST valid_formats)
			list (APPEND formatlist ${format})
		endif()
	endforeach()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()


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
    elseif (WIN32)
        set (formatlist Standalone Unity VST3)
    else()

        set (formatlist Standalone Unity VST3)

    endif()

    message (STATUS "Using default format list for platform: ${formatlist}")
    set (${outvar} ${formatlist} PARENT_SCOPE)

endfunction()