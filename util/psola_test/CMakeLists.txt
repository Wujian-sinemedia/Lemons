project (PSOLAtest VERSION 0.0.1 LANGUAGES CXX)

include (LemonsAudioModules)
include (LemonsTestingModules)

juce_add_plugin (PSOLAtest
    NEEDS_MIDI_INPUT                TRUE
    IS_SYNTH                        TRUE
    IS_MIDI_EFFECT                  FALSE
    DISABLE_AAX_MULTI_MONO          TRUE
    VST_NUM_MIDI_INS                1
    VST3_CATEGORIES                 "Pitch Shift"
    AU_MAIN_TYPE                    "kAudioUnitType_MusicEffect"
    MICROPHONE_PERMISSION_ENABLED   TRUE
    PLUGIN_NAME                     Psolatest
    PLUGIN_CODE                     Psla
    FORMATS                         ${LEMONS_PLUGIN_FORMATS}
    DESCRIPTION                     "Real-time vocal harmonizer instrument"
    )

lemons_configure_juce_plugin (TARGET ${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries (${PROJECT_NAME} PUBLIC lemons_psola lemons_unit_testing juce_audio_utils juce_audio_devices)
