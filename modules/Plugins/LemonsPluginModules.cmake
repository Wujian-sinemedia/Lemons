# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

# ======================================================================================
#
# ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
# ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
# ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
# ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
# ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
# ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
# This file is part of the Lemons open source library and is licensed under the terms of the GNU
# Public License.
#
# ======================================================================================

#[[
Building blocks for audio plugins

## Targets:
- LemonsPluginModules

]]

include_guard (GLOBAL)

include (LemonsPluginUtilities)
include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (TARGET LemonsPluginModules CATEGORY_DEPS LemonsCommonModules
								LemonsAudioModules LemonsGUIModules)

if("Standalone" IN_LIST "${LEMONS_PLUGIN_FORMATS}" OR "AUv3" IN_LIST "${LEMONS_PLUGIN_FORMATS}")
	target_link_libraries (lemons_plugin INTERFACE juce_audio_devices)
endif()
