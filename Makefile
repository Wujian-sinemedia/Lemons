SHELL := /usr/bin/env bash
.ONESHELL:
.SHELLFLAGS: -ec
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

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

logs: 
	mkdir logs

RUN_SCRIPT := scripts/run_cmake_in_dir.cmake

.PHONY: tests # Builds the tests
tests: | logs
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/tests -P $(RUN_SCRIPT)

.PHONY: run_tests # Runs all tests
run_tests: tests
	@echo " * make $@... * "
	cd util/tests/Builds/Debug && ctest -C Debug
	mv util/tests/Builds/Debug/Testing/Temporary/LastTest.log logs/UnitTests_Debug.log
	cd util/tests/Builds/Release && ctest -C Release
	mv util/tests/Builds/Release/Testing/Temporary/LastTest.log logs/UnitTests_Release.log

#

.PHONY: templates # Builds the project templates
templates: | logs
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/Templates -P $(RUN_SCRIPT)

#

.PHONY: editor # Builds the plugin metadata editor
editor: | logs
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/PluginMetadataEditor -P $(RUN_SCRIPT)

#

.PHONY: docs # Builds the documentation
docs: 
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/doxygen -P $(RUN_SCRIPT)

#

.PHONY: utils # Builds the command line utilities
utils: | logs
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/CommandLineUtils -P $(RUN_SCRIPT)

#

.PHONY: cmake_modules # Builds the cmake modules
cmake_modules: | logs
	@echo " * make $@... * "
	cmake -D LEMONS_DIR=util/cmake/modules -P $(RUN_SCRIPT)

.PHONY: install_cmake_modules # Builds and installs the cmake modules
install_cmake_modules: cmake_modules
	@echo " * make $@... * "
	cd util/cmake/modules && sudo cmake --install Builds

#

.PHONY: all # Builds and runs all CI
all: run_tests templates editor docs utils install_cmake_modules

#

.PHONY: help # Print this message
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20 | sort
