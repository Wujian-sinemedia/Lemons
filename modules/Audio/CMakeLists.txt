set (newmodules 
        bv_midi
        bv_dsp
        bv_audio_effects
        bv_synth
        bv_oscillators
        bv_psola)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)
