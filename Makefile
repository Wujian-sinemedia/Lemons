SHELL := /usr/bin/env bash

.PHONY: clean wipe format tests run_tests templates editor docs utils cmake_modules all

CMAKE_CONFIGURE_COMMAND := cmake -B Builds -G Xcode

CMAKE_BUILD_COMMAND := cmake --build Builds -j 4

#

clean:
	cmake -P scripts/clean.cmake

wipe:
	cmake -D LEMONS_WIPE_CLEAN=1 -P scripts/clean.cmake

format:
	cmake -P scripts/format.cmake

#

util/tests/Builds:
	cd util/tests && $(CMAKE_CONFIGURE_COMMAND)

tests: util/tests/Builds
	cd util/tests && $(CMAKE_BUILD_COMMAND)

run_tests: tests
	cd util/tests/Builds && ctest -C Debug

#

util/Templates/Builds:
	cd util/Templates && $(CMAKE_CONFIGURE_COMMAND)

templates: util/Templates/Builds
	cd util/Templates && $(CMAKE_BUILD_COMMAND)

#

util/PluginMetadataEditor/Builds:
	cd util/PluginMetadataEditor && $(CMAKE_CONFIGURE_COMMAND)

editor: util/PluginMetadataEditor/Builds
	cd util/PluginMetadataEditor && $(CMAKE_BUILD_COMMAND)

#

util/doxygen/Builds:
	cd util/doxygen && $(CMAKE_CONFIGURE_COMMAND)

docs: util/doxygen/Builds
	cd util/doxygen && $(CMAKE_BUILD_COMMAND)

#

util/CommandLineUtils/Builds:
	cd util/CommandLineUtils && $(CMAKE_CONFIGURE_COMMAND)

utils: util/CommandLineUtils/Builds
	cd util/CommandLineUtils && $(CMAKE_BUILD_COMMAND)

#

util/cmake/modules/Builds:
	cd util/cmake/modules && $(CMAKE_CONFIGURE_COMMAND)

cmake_modules: util/cmake/modules/Builds
	cd util/cmake/modules && $(CMAKE_BUILD_COMMAND)

#

all: run_tests templates editor docs utils cmake_modules
