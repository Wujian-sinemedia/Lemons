set (pluginval_repo_path "${CPM_SOURCE_CACHE}/pluginval")

#

function (_lemons_pull_or_clone_pluginval_repo)

    find_program (GIT git REQUIRED)

    if (IS_DIRECTORY "${pluginval_repo_path}")
        execute_process (COMMAND "${GIT}" "fetch" COMMAND "${GIT}" "pull"
                         WORKING_DIRECTORY "${pluginval_repo_path}")
    else()
        message (STATUS "Cloning pluginval repo...")
        execute_process (COMMAND "${GIT}" "clone" "--recurse-submodules" "https://github.com/Tracktion/pluginval.git" 
                         WORKING_DIRECTORY "${CPM_SOURCE_CACHE}")
    endif()
endfunction()

_lemons_pull_or_clone_pluginval_repo()

#

if (APPLE)
    set (pluginval_build_script "mac_build")
    set (pluginval_build_dir "MacOSX")
elseif (WIN32)
    set (pluginval_build_script "windows_build.bat")
    set (pluginval_build_dir "VisualStudio2017")
else()
    set (pluginval_build_script "linux_build")
    set (pluginval_build_dir "LinuxMakefile")
endif()

macro (_lemons_locate_pluginval)
    find_program (PLUGINVAL_EXEC pluginval PATHS "${pluginval_repo_path}/Builds/${pluginval_build_dir}/build/Release")
endmacro()

# see if we can find a previously-built version...
_lemons_locate_pluginval()

# Build pluginval
if (NOT PLUGINVAL_EXEC)
    unset (CACHE{PLUGINVAL_EXEC})

    message (STATUS "Building pluginval...")

    if (LEMONS_VST2_SDK_PATH)
        set (ENV{VST2_SDK_DIR} "${LEMONS_VST2_SDK_PATH}")
    endif()

    execute_process (COMMAND "./${pluginval_build_script}"
                     WORKING_DIRECTORY "${pluginval_repo_path}/install")

    _lemons_locate_pluginval()
endif()


################################################


function (_lemons_configure_pluginval_tests)

    if (NOT PLUGINVAL_EXEC)
        return()
    endif()

	set (options "")
    set (oneValueArgs TARGET LEVEL)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_PIV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    ###

    set (formats "VST")
    list (APPEND formats "AU")
    list (APPEND formats "VST3")

    set (atLeastOne FALSE)

    foreach (format ${formats})

        set (target "${LEMONS_PIV_TARGET}_${format}")

        if (NOT TARGET "${target}")
            continue()
        endif()

        set (atLeastOne TRUE)

        add_test (NAME "${target}-Pluginval" 
                  COMMAND "${PLUGINVAL_EXEC}" "--strictness-level" "${LEMONS_PIV_LEVEL}" "--validate" "$<TARGET_FILE:${target}>" 
                  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" 
                  COMMAND_EXPAND_LISTS)
    endforeach()

    if (atLeastOne)
        message (STATUS "Configured pluginval tests for target: ${LEMONS_PIV_TARGET}!")
    endif()
    
endfunction()
