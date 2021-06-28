set (newmodules 
        bv_core
        bv_mobile_utils
        bv_networking)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)