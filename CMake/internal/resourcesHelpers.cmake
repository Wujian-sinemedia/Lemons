
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

#

function (_add_resources_folder_dont_regenerate target folder)

    set (resourcesTarget ${CMAKE_PROJECT_NAME}_assets)
    
    if (NOT TARGET ${resourcesTarget})
        _create_resources_target (${resourcesTarget} ${folder})
    endif()

    _link_resources_target (${target} ${resourcesTarget})
endfunction()