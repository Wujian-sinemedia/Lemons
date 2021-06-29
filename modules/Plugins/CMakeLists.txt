set (newmodules 
        bv_plugin_parameters
        bv_plugin
        bv_plugin_gui)

juce_add_modules (${newmodules})

list (APPEND modules ${newmodules})

set (modules "${modules}" PARENT_SCOPE)