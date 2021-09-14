function (bv_get_target_source_files target exclude_pattern outvar)

    get_target_property (is_imported_target ${target} IMPORTED)
    if (is_imported_target)
        return()
    endif()

    get_target_property (sources "${target}" SOURCES)
    set (${target}_FILES "")

    foreach (source ${sources})
        get_source_file_property(lang "${source}" LANGUAGE)
        get_source_file_property(loc "${source}" LOCATION)

        if (NOT ("${lang}" MATCHES "C" OR "${lang}" MATCHES "CXX"))
            continue()
        endif()

        if (NOT ${loc} MATCHES ${exclude_pattern})
            list (APPEND ${target}_FILES "${loc}")
        endif()
    endforeach()

    set (${outvar} ${${target}_FILES} PARENT_SCOPE)

endfunction()