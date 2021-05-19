
function (_adjustDefaultMacTarget target bundleName)
    if (APPLE)
        set_target_properties (${target} PROPERTIES JUCE_BUNDLE_ID "com.bv.${bundleName}")

        if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
            set_target_properties (${target} PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY "./"
                    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                    XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
        endif ()
    endif ()
endfunction()

#

function (_create_resources_target targetName folder)
    file (GLOB_RECURSE files "${folder}/*.*")
    juce_add_binary_data (${targetName} SOURCES ${files})
    set_target_properties (${targetName} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    target_compile_definitions (${targetName} PUBLIC BV_HAS_BINARY_DATA=1)
endfunction()

#

function (_link_resources_target target resourcesTarget)
    _adjustDefaultMacTarget (${resourcesTarget} ${target})
    target_link_libraries (${target} PRIVATE ${resourcesTarget})
endfunction()

#

function (_add_resources_folder target folder)
    set (resourcesTarget ${target}-Assets)
    _create_resources_target (${resourcesTarget} ${folder})
    _link_resources_target (${target} ${resourcesTarget})
endfunction()


function (_add_resources_folder_dont_regenerate target folder)
    set (resourcesTarget ${CMAKE_PROJECT_NAME}_assets)
    
    if (NOT TARGET ${resourcesTarget})
        _create_resources_target (${resourcesTarget} ${folder})
    endif()

    _link_resources_target (${target} ${resourcesTarget})
endfunction()


#######


macro (_subdir_list result curdir)
    file (GLOB children RELATIVE ${curdir} ${curdir}/*)
    set (dirlist "")

    foreach (child ${children})
        if (IS_DIRECTORY ${curdir}/${child})
            list (APPEND dirlist ${child})
        endif ()
    endforeach ()

    set (${result} ${dirlist})
endmacro()

#

function (_add_subdirectory_if_valid subdir)
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory (${subdir})
    endif ()
endfunction()

#

function (_add_all_subdirs)
    _subdir_list (subDirs ${CMAKE_CURRENT_LIST_DIR})

    foreach (subdir ${subDirs})
        _add_subdirectory_if_valid (${subdir})
    endforeach()
endfunction()

#

function (_add_all_flagged_subdirs)
    _subdir_list (subDirs ${CMAKE_CURRENT_LIST_DIR})

    foreach (subdir ${subDirs})
        set (buildflag "Build${subdir}")

        if (${buildflag})
            _add_subdirectory_if_valid (${subdir})
        endif ()
    endforeach ()
endfunction()

