set (newmodules 
        bv_dsp
        bv_audio_effects
        bv_synth)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)
