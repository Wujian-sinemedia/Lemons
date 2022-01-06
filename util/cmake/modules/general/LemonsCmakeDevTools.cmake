include_guard (GLOBAL)


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
