
function (_bv_add_resources_folder target folder)
    set (resourcesTarget ${target}-Assets)

    if (NOT TARGET ${resourcesTarget})
        file (GLOB_RECURSE files "${folder}/*.*")
        juce_add_binary_data (${resourcesTarget} SOURCES ${files})
        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE BV_HAS_BINARY_DATA=1)
    endif()

    target_link_libraries (${target} PRIVATE ${resourcesTarget})
endfunction()
