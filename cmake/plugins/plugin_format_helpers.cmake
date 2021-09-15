

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

