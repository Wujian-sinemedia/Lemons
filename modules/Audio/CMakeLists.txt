set (newmodules 
        bv_midi)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

add_subdirectory (DSP)

set (modules "${modules}" PARENT_SCOPE)