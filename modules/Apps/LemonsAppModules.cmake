include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsAppUtilities)


_lemons_add_module_subcategory (LemonsAppModules)

target_link_libraries (LemonsAppModules INTERFACE LemonsCommonModules)
