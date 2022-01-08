#!/usr/bin/env cmake -P

include (@Lemons_SOURCE_DIR@/AddCMakeModules.cmake)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

include (LemonsRunClangFormat)

lemons_run_clang_format (DIRS @LEMONS_CF_DIRS@
						 EXCLUDE @LEMONS_CF_EXCLUDE_DIRS@)
