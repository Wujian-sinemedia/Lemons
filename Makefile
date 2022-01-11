SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -ec
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

.PHONY: help # Print this message
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20 | sort

#

CMAKE = cmake
CTEST = ctest

.PHONY: clean # Cleans the Lemons source tree
clean:
	$(CMAKE) -P scripts/clean.cmake

.PHONY: wipe # Wipes the persistent cache of fetched dependencies and ccache artifacts
wipe:
	$(CMAKE) -D LEMONS_WIPE_CLEAN=1 -P scripts/clean.cmake

.PHONY: format # Runs clang-format over the entire source tree
format:
	$(CMAKE) -P scripts/format.cmake

#

logs: 
	mkdir logs

run_cmake = $(CMAKE) -D LEMONS_DIR=$(1) -P scripts/run_cmake_in_dir.cmake

.PHONY: tests # Builds the tests
tests: | logs
	@echo " * make $@... * "
	$(call run_cmake,util/tests)

run_ctest = cd util/tests/Builds/$(1) && $(CTEST) -C $(1) \
		 $$ mv util/tests/Builds/$(1)/Testing/Temporary/LastTest.log logs/UnitTests_$(1).log

.PHONY: run_tests # Runs all tests
run_tests: tests
	@echo " * make $@... * "
	$(call run_ctest,Debug)
	$(call run_ctest,Release)

#

.PHONY: templates # Builds the project templates
templates: | logs
	@echo " * make $@... * "
	$(call run_cmake,util/Templates)

#

.PHONY: editor # Builds the plugin metadata editor
editor: | logs
	@echo " * make $@... * "
	$(call run_cmake,util/PluginMetadataEditor)

#

.PHONY: docs # Builds the documentation
docs: 
	@echo " * make $@... * "
	$(call run_cmake,util/doxygen)

#

.PHONY: utils # Builds the command line utilities
utils: | logs
	@echo " * make $@... * "
	$(call run_cmake,util/CommandLineUtils)

#

.PHONY: cmake_modules # Builds the cmake modules
cmake_modules: | logs
	@echo " * make $@... * "
	$(call run_cmake,util/cmake/modules)

.PHONY: install_cmake_modules # Builds and installs the cmake modules
install_cmake_modules: cmake_modules
	@echo " * make $@... * "
	cd util/cmake/modules && sudo $(CMAKE) --install Builds

#

.PHONY: all # Builds and runs all CI
all: run_tests templates editor docs utils install_cmake_modules

