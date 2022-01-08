include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)


##  Deprecations

function (_lemons_deprecated_variable_watch variableName access)
	if (access STREQUAL "READ_ACCESS")
		message (DEPRECATION "Read access of deprecated variable ${variableName}!")
	endif()
endfunction()


macro (lemons_deprecate_variable variableName)
	variable_watch (${variableName} _lemons_deprecated_variable_watch)
endmacro()


macro (lemons_deprecate_function functionName)
	if (NOT COMMAND ${functionName})
		message (AUTHOR_WARNING "Attempting to deprecate function ${functionName}, but command is not defined!")
	endif()

	function (${functionName})
		message (DEPRECATION "Deprecated function ${functionName} called!")
		cmake_language (CALL "_${functionName}" ${ARGN})
	endfunction()
endmacro()


#
##  Required function arguments

macro (lemons_require_function_arguments prefix)
	foreach (requiredArgument ${ARGN})
		if (NOT ${prefix}_${requiredArgument})
			message (FATAL_ERROR "Required argument ${requiredArgument} not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
		endif()
	endforeach()
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
