
_bv_subdir_list (categories ${CMAKE_CURRENT_LIST_DIR})

foreach (category ${categories})
	bv_add_juce_modules ("${CMAKE_CURRENT_LIST_DIR}/${category}")
endforeach()

set (modules "${modules}" PARENT_SCOPE)

#

if (DEFINED BV_IGNORE_VDSP)
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=0 BV_USE_VDSP=0)
elseif (APPLE)
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
endif()

_configure_mts_esp()