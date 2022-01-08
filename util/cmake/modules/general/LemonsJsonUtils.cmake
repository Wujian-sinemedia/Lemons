include_guard (GLOBAL)

include (LemonsCmakeDevTools)

function (lemons_json_array_to_list)

	set (oneValueArgs TEXT ARRAY OUT)

	cmake_parse_arguments (LEMONS_JSON "" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_JSON TEXT ARRAY OUT)

	string (JSON array_size ERROR_VARIABLE errno LENGTH ${LEMONS_JSON_TEXT} "${LEMONS_JSON_ARRAY}")

	if (errno)
		return()
	endif()

	math (EXPR array_size "${array_size} - 1" OUTPUT_FORMAT DECIMAL)

	set (out_list "")

	foreach (idx RANGE ${array_size})

		string (JSON item GET ${LEMONS_JSON_TEXT} "${LEMONS_JSON_ARRAY}" ${idx})

		list (APPEND out_list ${item})
		
	endforeach()

	set (${LEMONS_JSON_OUT} "${out_list}" PARENT_SCOPE)

endfunction()
