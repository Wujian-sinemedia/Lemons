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
#  ======================================================================================

#[[
Common building blocks and utilities

## Targets:
- LemonsCommonModules

]]

include_guard (GLOBAL)

#

include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (TARGET LemonsCommonModules)

target_compile_definitions (
	lemons_core
	INTERFACE LEMONS_VERSION_MAJOR="${Lemons_VERSION_MAJOR}"
			  LEMONS_VERSION_MINOR="${Lemons_VERSION_MINOR}"
			  LEMONS_VERSION_PATCH="${Lemons_VERSION_PATCH}"
			  LEMONS_PROJECT_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
			  LEMONS_PROJECT_VERSION_MINOR="${PROJECT_VERSION_MINOR}"
			  LEMONS_PROJECT_VERSION_PATCH="${PROJECT_VERSION_PATCH}")
