#!/usr/bin/env cmake -P

include (${CMAKE_CURRENT_LIST_DIR}/AddCMakeModules.cmake)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

if (NOT APPLE) # Homebrew takes forever to update installed deps on GH actions runners...
	set (LEMONS_DEPS_UPDATE_ALL_FIRST TRUE)
endif()

include (LemonsInstallDeps)


lemons_install_deps (FILE "${CMAKE_CURRENT_LIST_DIR}/Depsfile")
