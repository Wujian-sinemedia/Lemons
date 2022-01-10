include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)


macro (lemons_require_function_arguments prefix)
	foreach (requiredArgument ${ARGN})
		if (NOT ${prefix}_${requiredArgument})
			message (FATAL_ERROR "Required argument ${requiredArgument} not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
		endif()
	endforeach()
endmacro()


macro (lemons_check_for_unparsed_args prefix)
	if (${prefix}_UNPARSED_ARGUMENTS)
		message (FATAL_ERROR "Unparsed arguments ${${prefix}_UNPARSED_ARGUMENTS} found in call to ${CMAKE_CURRENT_FUNCTION}!")
	endif()
endmacro()


#

macro (lemons_warn_if_not_processing_project)
	# if (NOT CMAKE_ROLE STREQUAL "PROJECT")
 #    	message (AUTHOR_WARNING "This module (${CMAKE_CURRENT_LIST_FILE}) isn't meant to be used outside of project configurations. Some commands may not be available.")
	# endif()
endmacro()


#


function (lemons_append_to_target_property_list)

	set (oneValueArgs TARGET PROPERTY)

	cmake_parse_arguments (PROP_APPEND "" "${oneValueArgs}" "ADD" ${ARGN})

	lemons_require_function_arguments (PROP_APPEND TARGET PROPERTY ADD)

	get_target_property (orig_value ${PROP_APPEND_TARGET} ${PROP_APPEND_PROPERTY})

	if (NOT orig_value)
		set (orig_value "")
	endif()

	foreach (new_value ${PROP_APPEND_ADD})
		list (APPEND orig_value ${new_value})
	endforeach()

	list (REMOVE_DUPLICATES orig_value)

	set_target_properties (${PROP_APPEND_TARGET} PROPERTIES ${PROP_APPEND_PROPERTY} "${orig_value}")

endfunction()
