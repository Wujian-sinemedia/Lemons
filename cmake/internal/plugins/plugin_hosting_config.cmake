function (_bv_configure_plugin_hosting target)

    target_compile_definitions (${target} PRIVATE 
    		JUCE_PLUGINHOST_LADSPA=1
    		JUCE_PLUGINHOST_VST=0
    		JUCE_PLUGINHOST_VST3=1)

endfunction()