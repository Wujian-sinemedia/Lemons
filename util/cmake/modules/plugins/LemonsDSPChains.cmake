#[[
This module provides a function to autogenerate a header file defining an EngineChain class built with types described in a JSON file.

## Includes:
- LemonsAudioModules

## Functions:

### lemons_configure_dsp_chain
```
lemons_configure_dsp_chain (TARGET <target>
                            JSON <jsonFile>
                            [HEADER_NAME <headerName>])
```

If jsonFile is not an absolute path, it will be searched relative to PROJECT_SOURCE_DIR.

headerName may be omitted, and defaults to DSPChain.h.

The format of the JSON file must be:
```
{
    "DSP_chain": {
        "namespace": "DESIRED_NAMESPACE",
        "class_name": "DESIRED_CLASS_NAME",
        "nodes": [
            {
                "type": "TEMPLATED_ENGINE_TYPE"
            },
            {
                "type": "TEMPLATED_ENGINE_TYPE"
            }
        ]
    }
}
```

]]


include_guard (GLOBAL)

find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
    message (FATAL_ERROR "Python3 is required for code generation, but could not be found!")
endif()

include (LemonsAudioModules)


set (lemons_dsp_config_script "${CMAKE_CURRENT_LIST_DIR}/scripts/configure_signal_chain.py" CACHE INTERNAL "")


function (lemons_configure_dsp_chain)

    set (oneValueArgs JSON TARGET HEADER_NAME)

    cmake_parse_arguments (LEMONS_GEN "" "${oneValueArgs}" "" ${ARGN})

    if (NOT LEMONS_GEN_JSON)
    	message (FATAL_ERROR "JSON not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    cmake_path (IS_ABSOLUTE LEMONS_GEN_JSON json_path_is_absolute)

    if (json_path_is_absolute)
        set (json_file "${LEMONS_GEN_JSON}")
    else()
        set (json_file "${PROJECT_SOURCE_DIR}/${LEMONS_GEN_JSON}")
    endif()

    if (NOT EXISTS "${json_file}")
    	message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Nonexistant input JSON file!")
    endif()

    if (NOT LEMONS_GEN_TARGET)
    	message (FATAL_ERROR "TARGET not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    if (NOT TARGET ${LEMONS_GEN_TARGET})
    	message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Target ${LEMONS_GEN_TARGET} does not exist!")
    endif()

    if (NOT LEMONS_GEN_HEADER_NAME)
        set (LEMONS_GEN_HEADER_NAME "DSPChain.h")
    endif()

    set (generated_header "${CMAKE_CURRENT_BINARY_DIR}/${LEMONS_GEN_HEADER_NAME}")

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${lemons_dsp_config_script}" "${generated_header}" "${json_file}")

    target_sources (${LEMONS_GEN_TARGET} PUBLIC "${generated_header}")

    target_link_libraries (${LEMONS_GEN_TARGET} PUBLIC lemons_dsp)

endfunction()
