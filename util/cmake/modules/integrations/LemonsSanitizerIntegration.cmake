#[[
Configures various sanitizers for the current build.

## Options:
LEMONS_SANITIZERS_TO_ENABLE: a list of sanitizers to enable.
The options are:
- address
- memory
- thread
- leak
- undefined
- fuzzer

The sanitizers "address", "memory" and "thread" are mutually exclusive.  You cannot enable two of them in the same build.

"leak" requires the  "address" sanitizer.

]]


if (NOT LEMONS_SANITIZERS_TO_ENABLE)
	return()
endif()


if (NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU" 
      OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" 
      OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC"))
    message (VERBOSE "Sanitizers not supported with your current compiler: ${CMAKE_CXX_COMPILER_ID}")
    return()
endif()

#

function (_lemons_enable_sanitizer_flags sanitize_option)

    macro (_lemons_check_compiler_version)

        macro (_lemons_compiler_version_failed)
            message (WARNING "${sanitizer} could not be enabled, because your compiler (${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}) does not support it!")
            return()
        endmacro()

        set (oneValueArgs GCC CLANG MSVC)
        cmake_parse_arguments (LEMONS_SAN "" "${oneValueArgs}" "" ${ARGN})

        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            if (LEMONS_SAN_GCC)
                if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${LEMONS_SAN_GCC})
                    _lemons_compiler_version_failed()
                endif()
            endif()
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            if (LEMONS_SAN_CLANG)
                if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${LEMONS_SAN_CLANG})
                    _lemons_compiler_version_failed()
                endif()
            endif()
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            if (LEMONS_SAN_MSVC)
                if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${LEMONS_SAN_MSVC})
                    _lemons_compiler_version_failed()
                endif()
            endif()
        endif()
    endmacro()

    #

    if (${sanitize_option} MATCHES "address")
        _lemons_check_compiler_version (GCC "4.8" CLANG "3.1" MSVC "19.28")

        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set (XSAN_COMPILE_FLAGS "-fsanitize=address" PARENT_SCOPE)
        else()
            set (XSAN_COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls" PARENT_SCOPE)
            set (XSAN_LINKER_FLAGS "asan" PARENT_SCOPE)
        endif()

        return()
    endif()

    if (${sanitize_option} MATCHES "thread")
        _lemons_check_compiler_version (GCC "4.8" CLANG "3.1")

        set (XSAN_COMPILE_FLAGS "-fsanitize=thread" PARENT_SCOPE)
        set (XSAN_LINKER_FLAGS "tsan" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "memory")
        _lemons_check_compiler_version (CLANG "3.1")

        set (XSAN_COMPILE_FLAGS "-fsanitize=memory" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "leak")
        _lemons_check_compiler_version (GCC "4.9" CLANG "3.4")

        set (XSAN_COMPILE_FLAGS "-fsanitize=leak" PARENT_SCOPE)
        set (XSAN_LINKER_FLAGS "lsan" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "undefined")
        _lemons_check_compiler_version (GCC "4.9" CLANG "3.1")

        set (XSAN_COMPILE_FLAGS "-fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "fuzzer")
        _lemons_check_compiler_version (CLANG "6.0")

        set (XSAN_COMPILE_FLAGS "-fsanitize=fuzzer" PARENT_SCOPE)

        return()
    endif()

    message (WARNING "Unknown sanitizer requested: ${sanitize_option}")
endfunction()

#

foreach (sanitizer ${LEMONS_SANITIZERS_TO_ENABLE})

    string (TOLOWER ${sanitizer} sanitizer)

    unset (XSAN_COMPILE_FLAGS)
    unset (XSAN_LINKER_FLAGS)

    _lemons_enable_sanitizer_flags (${sanitizer})

    if (NOT XSAN_COMPILE_FLAGS AND NOT XSAN_LINKER_FLAGS)
        continue()
    endif()

    list (APPEND CMAKE_CXX_FLAGS ${XSAN_COMPILE_FLAGS})

    if (XSAN_LINKER_FLAGS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        link_libraries (${XSAN_LINKER_FLAGS})
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        list (APPEND CMAKE_C_FLAGS ${XSAN_COMPILE_FLAGS})
        string (REPLACE "-Wl,--no-undefined" "" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
        string (REPLACE "-Wl,--no-undefined" "" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
    endif()
endforeach()
