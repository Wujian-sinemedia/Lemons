set (newmodules 
        bv_core
        bv_midi
        bv_mobile_utils
        bv_plugin
        bv_networking)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

add_subdirectory (DSP)

add_subdirectory (GUI)

set (modules "${modules}" PARENT_SCOPE)