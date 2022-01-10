#!/usr/bin/env cmake -P

include (${CMAKE_CURRENT_LIST_DIR}/AddCMakeModules.cmake)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

if (NOT APPLE AND NOT WIN32)
	set (LEMONS_DEPS_UPDATE_ALL_FIRST TRUE)
endif()

include (LemonsInstallDeps)


option (LEMONS_DOCS "Install dependencies for building docs" OFF)
option (LEMONS_DOCKER "Install dependencies for Docker" OFF)

if (LEMONS_DOCS)
	lemons_install_deps (FILE lemons_config.json CATEGORIES Docs)
elseif ()
	lemons_install_deps (FILE lemons_config.json CATEGORIES Docker)
else()
	lemons_install_deps (FILE lemons_config.json)
endif()

