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

if (NOT DEFINED BV_SANITIZERS_TO_ENABLE)
	set (BV_SANITIZERS_TO_ENABLE "")
endif()


macro (check_compiler_version gcc_required_version clang_required_version msvc_required_version)

	if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${gcc_required_version}
		OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${clang_required_version}
		OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${msvc_required_version})

		message (FATAL_ERROR "${sanitizer} could not be enabled, because your compiler (${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}) does not support it!")
	endif()
endmacro()


macro (enable_sanitizer_flags sanitize_option)
    if (${sanitize_option} MATCHES "address")
        check_compiler_version ("4.8" "3.1" "19.28")
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set (XSAN_COMPILE_FLAGS "-fsanitize=address")
        else()
            set (XSAN_COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
            set (XSAN_LINKER_FLAGS "asan")
        endif()
        return()
    endif()

    if (${sanitize_option} MATCHES "thread")
        check_compiler_version ("4.8" "3.1" "99.99")
        set (XSAN_COMPILE_FLAGS "-fsanitize=thread")
        set (XSAN_LINKER_FLAGS "tsan")
        return()
    endif()

    if (${sanitize_option} MATCHES "memory")
        check_compiler_version ("99.99" "3.1" "99.99")
        set (XSAN_COMPILE_FLAGS "-fsanitize=memory")
        return()
    endif()

    if (${sanitize_option} MATCHES "leak")
        check_compiler_version ("4.9" "3.4" "99.99")
        set (XSAN_COMPILE_FLAGS "-fsanitize=leak")
        set (XSAN_LINKER_FLAGS "lsan")
        return()
    endif()

    if (${sanitize_option} MATCHES "undefined")
        check_compiler_version ("4.9" "3.1" "99.99")
        set (XSAN_COMPILE_FLAGS "-fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls")
    endif()

    if (${sanitize_option} MATCHES "fuzzer")
        check_compiler_version ("99.99" "6.0" "99.99")
        set (XSAN_COMPILE_FLAGS "-fsanitize=fuzzer")
    endif()
endmacro()


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        
    foreach (sanitizer ${BV_SANITIZERS_TO_ENABLE})

        string (TOLOWER ${sanitizer} sanitizer)

        enable_sanitizer_flags (${sanitizer})

        if (CMAKE_C_COMPILER_ID MATCHES "Clang")
          set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${XSAN_COMPILE_FLAGS}")
        endif()

        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${XSAN_COMPILE_FLAGS}")

        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
          link_libraries (${XSAN_LINKER_FLAGS})
        endif()

        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            string (REPLACE "-Wl,--no-undefined" "" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
            string (REPLACE "-Wl,--no-undefined" "" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
        endif()
    endforeach()
else()
	message (WARNING "Your compiler (${CMAKE_CXX_COMPILER_ID}) does not have sanitizer support :(")
endif()
