set (newmodules 
        bv_app_utils)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)