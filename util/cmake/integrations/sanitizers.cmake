#[[

The code in this file is based on: https://github.com/KDE/extra-cmake-modules/blob/master/modules/ECMEnableSanitizers.cmake

The options are:

- address
- memory
- thread
- leak
- undefined
- fuzzer

The sanitizers "address", "memory" and "thread" are mutually exclusive.  You
cannot enable two of them in the same build.

"leak" requires the  "address" sanitizer.

]]


if (NOT LEMONS_SANITIZERS_TO_ENABLE)
	return()
endif()


if (NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU" 
      OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" 
      OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC"))
    return()
endif()


function (_lemons_enable_sanitizer_flags sanitize_option)

    macro (_lemons_check_compiler_version gcc_required_version clang_required_version msvc_required_version)

        if ( (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${gcc_required_version})
            OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${clang_required_version})
            OR (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${msvc_required_version}) )

            message (WARNING "${sanitizer} could not be enabled, because your compiler (${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}) does not support it!")
            return()
        endif()
    endmacro()


    if (${sanitize_option} MATCHES "address")
        _lemons_check_compiler_version ("4.8" "3.1" "19.28")

        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set (XSAN_COMPILE_FLAGS "-fsanitize=address" PARENT_SCOPE)
        else()
            set (XSAN_COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls" PARENT_SCOPE)
            set (XSAN_LINKER_FLAGS "asan" PARENT_SCOPE)
        endif()

        return()
    endif()

    if (${sanitize_option} MATCHES "thread")
        _lemons_check_compiler_version ("4.8" "3.1" "99.99")

        set (XSAN_COMPILE_FLAGS "-fsanitize=thread" PARENT_SCOPE)
        set (XSAN_LINKER_FLAGS "tsan" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "memory")
        _lemons_check_compiler_version ("99.99" "3.1" "99.99")

        set (XSAN_COMPILE_FLAGS "-fsanitize=memory" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "leak")
        _lemons_check_compiler_version ("4.9" "3.4" "99.99")

        set (XSAN_COMPILE_FLAGS "-fsanitize=leak" PARENT_SCOPE)
        set (XSAN_LINKER_FLAGS "lsan" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "undefined")
        _lemons_check_compiler_version ("4.9" "3.1" "99.99")

        set (XSAN_COMPILE_FLAGS "-fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls" PARENT_SCOPE)

        return()
    endif()

    if (${sanitize_option} MATCHES "fuzzer")
        _lemons_check_compiler_version ("99.99" "6.0" "99.99")

        set (XSAN_COMPILE_FLAGS "-fsanitize=fuzzer" PARENT_SCOPE)

        return()
    endif()

    message (WARNING "Unknown sanitizer requested: ${sanitize_option}")
endfunction()

#

foreach (sanitizer ${LEMONS_SANITIZERS_TO_ENABLE})

    string (TOLOWER ${sanitizer} sanitizer)

    _lemons_enable_sanitizer_flags (${sanitizer})

    if (CMAKE_C_COMPILER_ID MATCHES "Clang")
        list (APPEND CMAKE_C_FLAGS ${XSAN_COMPILE_FLAGS})
    endif()

    list (APPEND CMAKE_CXX_FLAGS ${XSAN_COMPILE_FLAGS})

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        link_libraries (${XSAN_LINKER_FLAGS})
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        string (REPLACE "-Wl,--no-undefined" "" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
        string (REPLACE "-Wl,--no-undefined" "" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
    endif()
endforeach()
