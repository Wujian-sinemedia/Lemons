#[[
# LemonsPluginvalUtils  {#LemonsPluginvalUtils}

## Include-time actions:
Adds pluginval using CPM.cmake, and runs their provided platform-specific build-and-install script.

## Functions:
```
lemons_configure_pluginval_tests (TARGET <target>)
```
Note that this does nothing if you don't manually call the enable_testing() macro first.

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
endif()


################################################


function (lemons_configure_pluginval_tests)

    if (NOT PLUGINVAL_EXEC)
        return()
    endif()

	set (options "")
    set (oneValueArgs TARGET)
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

        set (testName "${target}-Pluginval")

        add_test (NAME "${testName}" 
                  COMMAND "${PLUGINVAL_EXEC}" "--strictness-level" "${LEMONS_PLUGINVAL_LEVEL}" "--validate" "$<TARGET_PROPERTY:${target},JUCE_PLUGIN_ARTEFACT_FILE>" "--validate-in-process"
                  COMMAND_EXPAND_LISTS)

        set_tests_properties ("${testName}" PROPERTIES 
                              REQUIRED_FILES "$<TARGET_PROPERTY:${target},JUCE_PLUGIN_ARTEFACT_FILE>;${PLUGINVAL_EXEC}"
                              LABELS Pluginval)

        set (atLeastOne TRUE)

    endforeach()

    if (atLeastOne)
        message (VERBOSE "Configured pluginval tests for target: ${LEMONS_PIV_TARGET}!")
    endif()
    
endfunction()
