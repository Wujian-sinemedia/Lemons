function (_bv_add_juce_modules_internal dir list)

	_bv_subdir_list (moduleFolders ${dir})

    foreach (folder ${moduleFolders})
        juce_add_module (${dir}/${folder})
    endforeach()

    list (APPEND ${list} ${moduleFolders} PARENT_SCOPE)

endfunction()