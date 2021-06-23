set (newmodules 
        ableton_push_display
        bv_gui)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)