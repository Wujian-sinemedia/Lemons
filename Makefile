SHELL := /usr/bin/env bash

#

.PHONY: clean # Cleans the Lemons source tree
clean:
	cmake -P scripts/clean.cmake

.PHONY: wipe # Wipes the persistent cache of fetched dependencies and ccache artifacts
wipe:
	cmake -D LEMONS_WIPE_CLEAN=1 -P scripts/clean.cmake

.PHONY: format # Runs clang-format over the entire source tree
format:
	cmake -P scripts/format.cmake

#

RUN_SCRIPT := scripts/run_cmake_in_dir.cmake

.PHONY: tests # Builds the tests
tests:
	cmake -D LEMONS_DIR=util/tests -P $(RUN_SCRIPT)

.PHONY: run_tests # Runs all tests
run_tests: tests
	cd util/tests/Builds && ctest -C Debug

#

.PHONY: templates # Builds the project templates
templates: 
	cmake -D LEMONS_DIR=util/Templates -P $(RUN_SCRIPT)

#

.PHONY: editor # Builds the plugin metadata editor
editor: 
	cmake -D LEMONS_DIR=util/PluginMetadataEditor -P $(RUN_SCRIPT)

#

.PHONY: docs # Builds the documentation
docs: 
	cmake -D LEMONS_DIR=util/doxygen -P $(RUN_SCRIPT)

#

.PHONY: utils # Builds the command line utilities
utils: util/CommandLineUtils/Builds
	cmake -D LEMONS_DIR=util/CommandLineUtils -P $(RUN_SCRIPT)

#

.PHONY: cmake_modules # Builds the cmake modules
cmake_modules: util/cmake/modules/Builds
	cmake -D LEMONS_DIR=util/cmake/modules -P $(RUN_SCRIPT)

.PHONY: install_cmake_modules # Builds and installs the cmake modules
install_cmake_modules: cmake_modules
	cd util/cmake/modules && sudo cmake --install Builds

#

.PHONY: all # Builds and runs all CI
all: run_tests templates editor docs utils install_cmake_modules

#

.PHONY: help # Print this message
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20 | sort
