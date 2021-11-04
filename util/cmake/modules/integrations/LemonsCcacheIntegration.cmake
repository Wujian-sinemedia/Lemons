include_guard (GLOBAL)

#[[
    The code in this file is based on:
        - this blog post: https://cristianadam.eu/20200113/speeding-up-c-plus-plus-github-actions-using-ccache/
        - and this repository: https://github.com/TheLartians/Ccache.cmake

    Configures ccache support for all builds in this source tree. 
    (Specifically, works by configuring a shell script that invokes ccache with the correct arguments for each build, then sets that shell script as the C and C++ launcher for these builds.)
    
    Ccache can drastically improve build time, and there's not really a good reason NOT to use it if it's installed on your system.
]]

find_program (CCACHE_PROGRAM ccache)

if (NOT CCACHE_PROGRAM)
    return()
endif()

set  (ccache_options "CCACHE_BASEDIR=${CMAKE_SOURCE_DIR}")

if (DEFINED ENV{CPM_SOURCE_CACHE})
    list (APPEND ccache_options "CCACHE_DIR=$ENV{CPM_SOURCE_CACHE}/ccache/cache")
else()
    list (APPEND ccache_options "CCACHE_DIR=${CMAKE_SOURCE_DIR}/Cache/ccache/cache")
endif()

list (APPEND ccache_options "CCACHE_COMPRESS=true")
list (APPEND ccache_options "CCACHE_COMPRESSLEVEL=6")
list (APPEND ccache_options "CCACHE_MAXSIZE=800M")

list (JOIN ccache_options "\n export " CCCACHE_EXPORTS)

function (_lemons_configure_compiler_launcher language)

    string (TOUPPER "${language}" lang_upper)

    set (CCACHE_COMPILER_BEING_CONFIGURED "${CMAKE_${lang_upper}_COMPILER}")

    set (script_name "launch-${language}")

    configure_file ("${CMAKE_CURRENT_LIST_DIR}/launcher.in" "${script_name}")

    set (${language}_script "${CMAKE_CURRENT_BINARY_DIR}/${script_name}" PARENT_SCOPE)
endfunction()

_lemons_configure_compiler_launcher (c)
_lemons_configure_compiler_launcher (cxx)

execute_process (COMMAND chmod a+rx "${c_script}" "${cxx_script}")

if (XCODE)
    set (CMAKE_XCODE_ATTRIBUTE_CC         "${c_script}" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_CXX        "${cxx_script}" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_LD         "${c_script}" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${cxx_script}" CACHE INTERNAL "")
else()
    set (CMAKE_C_COMPILER_LAUNCHER   "${c_script}" CACHE INTERNAL "")
    set (CMAKE_CXX_COMPILER_LAUNCHER "${cxx_script}" CACHE INTERNAL "")
endif()

message (VERBOSE " -- Using ccache! -- ")
