#[[
Utilities for testing audio plugins with pluginval.

## Include-time actions:
Adds pluginval using CPM.cmake, and runs their provided platform-specific build-and-install script.

## Functions:

### lemons_configure_pluginval_tests {#lemons_configure_pluginval_tests}
```
lemons_configure_pluginval_tests (TARGET <target>)
```
Creates a pluginval test target for the given plugin.

Note that this does nothing if you don't manually call the enable_testing() macro first.

#### Options
- LEMONS_PLUGINVAL_LEVEL: The "level" of strictness of the configured pluginval tests; 1 through 10. Defaults to 10.

]]


include_guard (GLOBAL)

CPMAddPackage (
        NAME pluginval
        GITHUB_REPOSITORY Tracktion/pluginval
        GIT_TAG origin/develop
        DOWNLOAD_ONLY YES)

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
    find_program (PLUGINVAL_EXEC pluginval PATHS "${pluginval_SOURCE_DIR}/Builds/${pluginval_build_dir}/build/Release")
endmacro()

_lemons_locate_pluginval()

if (NOT PLUGINVAL_EXEC)
    unset (CACHE{PLUGINVAL_EXEC})

    message (STATUS "Building pluginval...")

    if (LEMONS_VST2_SDK_PATH)
        set (ENV{VST2_SDK_DIR} "${LEMONS_VST2_SDK_PATH}")
    endif()

    execute_process (COMMAND "./${pluginval_build_script}"
                     WORKING_DIRECTORY "${pluginval_SOURCE_DIR}/install")

    _lemons_locate_pluginval()

    if (NOT PLUGINVAL_EXEC)
        message (WARNING "Error building pluginval - executable cannot be located!")
    endif()
endif()

set (LEMONS_PLUGINVAL_LEVEL "10" CACHE STRING "Pluginval testing level")


################################################


function (lemons_configure_pluginval_tests)

    if (NOT PLUGINVAL_EXEC)
        return()
    endif()

    set (oneValueArgs TARGET)
    cmake_parse_arguments (LEMONS_PIV "" "${oneValueArgs}" "" ${ARGN})

    ###

    set (formats VST AU VST3)
    set (atLeastOne FALSE)

    foreach (format ${formats})
        set (target "${LEMONS_PIV_TARGET}_${format}")

        if (NOT TARGET "${target}")
            continue()
        endif()

        set (testName "${target}-Pluginval")

        add_test (NAME ${testName}
                  COMMAND "${PLUGINVAL_EXEC}" --strictness-level ${LEMONS_PLUGINVAL_LEVEL} --validate "$<TARGET_PROPERTY:${target},JUCE_PLUGIN_ARTEFACT_FILE>" --validate-in-process
                  COMMAND_EXPAND_LISTS)

        set_tests_properties (${testName} PROPERTIES 
                              REQUIRED_FILES "$<TARGET_PROPERTY:${target},JUCE_PLUGIN_ARTEFACT_FILE>;${PLUGINVAL_EXEC}"
                              LABELS Pluginval)

        set (atLeastOne TRUE)

        message (VERBOSE "Configured pluginval tests for format ${format} of plugin ${LEMONS_PIV_TARGET}: target ${target}")
    endforeach()

    if (atLeastOne)
        message (STATUS "Configured pluginval tests for target: ${LEMONS_PIV_TARGET}!")
    endif()
endfunction()
