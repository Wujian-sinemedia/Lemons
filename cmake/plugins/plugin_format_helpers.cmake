function (_bv_create_default_format_list_for_platform outlist)

	set (formatlist Standalone)

	if (NOT "${CMAKE_SYSTEM_NAME}" MATCHES "iOS")

		list (APPEND formatlist Unity VST3)

		if (APPLE)
			list (APPEND formatlist AU AUv3)
		endif()

		if (${BV_LV2_AVAILABLE})
			list (APPEND formatlist LV2)
		endif()

		if (${BV_AAX_AVAILABLE})
			list (APPEND formatlist AAX)
		endif()

		if (${BV_VST2_AVAILABLE})
			list (APPEND formatlist VST)
		endif()

	endif()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()

#

function (_bv_parse_plugin_format_list outlist inlist)

	if ("${inlist}" STREQUAL "")
		set (${outlist} "" PARENT_SCOPE)
		return()
	endif()

	_bv_create_default_format_list_for_platform (valid_formats)

	set (formatlist "")

	foreach (format ${inlist})
		if ("${format}" IN_LIST valid_formats)
			list (APPEND formatlist ${format})
		else()
			message (STATUS "Unsupported format removed from list: ${format}")
		endif()
	endforeach()

	set (${outlist} ${formatlist} PARENT_SCOPE)

endfunction()

#

function (bv_detect_plugin_formats outvar)
	_bv_create_default_format_list_for_platform (formatlist)
	set (${outvar} ${formatlist} PARENT_SCOPE)
endfunction()

