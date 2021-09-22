function (_lemons_configure_plugin_hosting target)

    target_compile_definitions (${target} PRIVATE 
            JUCE_PLUGINHOST_VST=0
            JUCE_PLUGINHOST_VST3=1
    		JUCE_PLUGINHOST_LADSPA=1)

    if (APPLE)
        target_compile_definitions (${target} PRIVATE JUCE_PLUGINHOST_AU=1)
    endif()
    
endfunction()