_bv_add_juce_modules_internal ("${CMAKE_CURRENT_LIST_DIR}/Plugins" plugin_only_modules)

_bv_add_juce_modules_internal ("${CMAKE_CURRENT_LIST_DIR}/Apps" app_only_modules)

#

_bv_subdir_list (categories ${CMAKE_CURRENT_LIST_DIR})

foreach (category ${categories})
    if (${category} STREQUAL "" OR ${category} STREQUAL "Plugins" OR ${category} STREQUAL "Apps")
        continue()
    endif()

	_bv_add_juce_modules_internal ("${CMAKE_CURRENT_LIST_DIR}/${category}" modules)
endforeach()

#

set (BV_JUCE_MODULES "${modules}" PARENT_SCOPE)

set (BV_PLUGIN_ONLY_MODULES "${plugin_only_modules}" PARENT_SCOPE)

set (BV_APP_ONLY_MODULES "${app_only_modules}" PARENT_SCOPE)

#

if (DEFINED BV_IGNORE_VDSP)
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=0 BV_USE_VDSP=0)
elseif (APPLE)
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
endif()

_configure_mts_esp()