function (_bv_add_juce_modules_internal dir list)
    bv_subdir_list (RESULT moduleFolders DIR ${dir})

    foreach (folder ${moduleFolders})
        juce_add_module (${dir}/${folder})
    endforeach()

    list (APPEND ${list} ${moduleFolders} PARENT_SCOPE)
endfunction()

macro (_bv_add_module_subcategory folder list)
    _bv_add_juce_modules_internal ("${CMAKE_CURRENT_LIST_DIR}/${folder}" ${list})
    set (${list} ${${list}} PARENT_SCOPE)
endmacro()

_bv_add_module_subcategory (Plugins      BV_PLUGIN_ONLY_MODULES)
_bv_add_module_subcategory (Audio        BV_PLUGIN_ONLY_MODULES)
_bv_add_module_subcategory (Apps         BV_APP_ONLY_MODULES)
_bv_add_module_subcategory (Common       BV_JUCE_MODULES)
_bv_add_module_subcategory (Connectivity BV_JUCE_MODULES)
_bv_add_module_subcategory (GUI          BV_JUCE_MODULES)

#

target_link_libraries (bv_midi   INTERFACE MTS-ESP)
target_link_libraries (bv_plugin INTERFACE AbletonLink)

#

function (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} dummylist)
endfunction()