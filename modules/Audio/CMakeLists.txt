set (newmodules 
        bv_midi
        bv_plugin)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

add_subdirectory (DSP)

set (modules "${modules}" PARENT_SCOPE)