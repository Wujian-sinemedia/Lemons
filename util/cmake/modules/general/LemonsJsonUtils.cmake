include_guard (GLOBAL)

include (LemonsCmakeDevTools)

#[[ Populate a list variable in the calling scope with all the values from a an array of a JSON object.

```
lemons_json_array_to_list (TEXT <jsonString>
						   ARRAY <nameOfArray>
						   OUT <outputVar>)
```

If the array is not present in the passed JSON string, the output variable will be empty. 

Example usage:

in myJson.json:
```
{
	"Names": [ "Bob", "Harry", "Sally" ]
}
```

```
file (READ myJson.json jsonContents)

lemons_json_array_to_list (TEXT ${jsonContents} ARRAY "Names" OUT jsonNames)

# the variable jsonNames now contains "Bob;Harry;Sally"

lemons_json_array_to_list (TEXT ${jsonContents} ARRAY "Ages" OUT jsonAges)

if (NOT jsonAges)
	# this block will be executed, because the array will not be found in the JSON
endif()
```

]]
function (lemons_json_array_to_list)

	set (oneValueArgs TEXT ARRAY OUT)

	cmake_parse_arguments (LEMONS_JSON "" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_JSON TEXT ARRAY OUT)
	lemons_check_for_unparsed_args (LEMONS_JSON)

	string (JSON array_size ERROR_VARIABLE errno LENGTH ${LEMONS_JSON_TEXT} "${LEMONS_JSON_ARRAY}")

	if (errno)
		message (WARNING "Error parsing JSON string for array '${LEMONS_JSON_ARRAY}': ${LEMONS_JSON_TEXT}")
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
