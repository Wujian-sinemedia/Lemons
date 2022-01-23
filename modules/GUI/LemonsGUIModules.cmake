#[[
Building blocks for GUIs

## Targets:
- LemonsGUIModules

]]

include_guard (GLOBAL)

include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (TARGET LemonsGUIModules CATEGORY_DEPS
								LemonsCommonModules)
