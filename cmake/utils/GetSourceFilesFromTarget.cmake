function (bv_get_target_source_files target exclude_pattern outvar)

  get_target_property (is_imported_target ${target} IMPORTED)
  if (is_imported_target)
    return()
  endif()

  get_target_property (_sources "${target}" SOURCES)
  set (${target}_FILES "")

  foreach (source ${_sources})
    get_source_file_property(lang "${source}" LANGUAGE)
    get_source_file_property(loc "${source}" LOCATION)

    if (NOT ("${lang}" MATCHES "C" OR "${lang}" MATCHES "CXX"))
      continue()
    endif()

    if (${loc} MATCHES ${exclude_pattern})
      continue()
    endif()

    list (APPEND ${target}_FILES "${loc}")
  endforeach()

  set (${outvar} ${${target}_FILES} PARENT_SCOPE)

endfunction()