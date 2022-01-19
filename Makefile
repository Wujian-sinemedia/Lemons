SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo pipefail
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

CMAKE = cmake
CTEST = ctest

LEMONS_ROOT = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

BUILDS = Builds

ifeq ($(OS),Windows_NT)
	NUM_CORES = $(NUMBER_OF_PROCESSORS)
	CMAKE_GENERATOR = Visual Studio 16 2019
else ifeq ($(shell uname -s),Darwin)
	NUM_CORES = $(shell sysctl hw.ncpu | awk '{print $$2}')
	CMAKE_GENERATOR = Xcode
else
	NUM_CORES = $(shell grep -c ^processor /proc/cpuinfo)
	CMAKE_GENERATOR = Ninja
endif

#

cmake_config = cd $(LEMONS_ROOT)/$(1) && $(CMAKE) -B $(BUILDS) -G $(CMAKE_GENERATOR) --log-level=DEBUG

cmake_build = cd $(LEMONS_ROOT)/$(1) \
		   && echo "Building debug configuration..." && $(CMAKE) --build $(BUILDS) -j $(NUM_CORES) --config Debug \
		   && echo "Building release configuration..." && $(CMAKE) --build $(BUILDS) -j $(NUM_CORES) --config Release

#

.PHONY: help # Print this message
help:
	@grep '^.PHONY: .* #' $(LEMONS_ROOT)/Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20 | sort

#

$(LEMONS_ROOT)/util/tests/$(BUILDS):
	$(call cmake_config,util/tests)

.PHONY: tests # Builds the tests
tests: $(LEMONS_ROOT)/util/tests/$(BUILDS)
	@$(call cmake_build,util/tests)

.PHONY: run_tests # Runs all tests
run_tests: tests
	@echo "Testing debug configuration..."
	@cd $(LEMONS_ROOT)/util/tests/Builds && $(CTEST) -C Debug --output-on-failure
	@echo "Testing release configuration..."
	@cd $(LEMONS_ROOT)/util/tests/Builds && $(CTEST) -C Release --output-on-failure

.PHONY: cppcheck # Runs cppcheck
cppcheck:
	@echo " * make $@... * "
	$(CMAKE) -P $(LEMONS_ROOT)/scripts/run_cppcheck.cmake

#

$(LEMONS_ROOT)/util/Templates/$(BUILDS):
	$(call cmake_config,util/Templates)

.PHONY: templates # Builds the project templates
templates: $(LEMONS_ROOT)/util/Templates/$(BUILDS)
	@$(call cmake_build,util/Templates)

#

$(LEMONS_ROOT)/util/PluginMetadataEditor/$(BUILDS):
	$(call cmake_config,util/PluginMetadataEditor)

.PHONY: editor # Builds the plugin metadata editor
editor: $(LEMONS_ROOT)/util/PluginMetadataEditor/$(BUILDS)
	@$(call cmake_build,util/PluginMetadataEditor)

#

$(LEMONS_ROOT)/util/CommandLineUtils/$(BUILDS):
	$(call cmake_config,util/CommandLineUtils)

.PHONY: utils # Builds the command line utilities
utils: $(LEMONS_ROOT)/util/CommandLineUtils/$(BUILDS)
	@$(call cmake_build,util/CommandLineUtils)

#

.PHONY: docs # Builds the documentation
docs:
	$(call cmake_config,util/doxygen)
	@cd $(LEMONS_ROOT)/util/doxygen && $(CMAKE) --build $(BUILDS) -j $(NUM_CORES)

#

.PHONY: all # Builds everything
all: tests templates editor utils docs

#

.PHONY: clean # Cleans the Lemons source tree
clean:
	$(CMAKE) -P $(LEMONS_ROOT)/scripts/clean.cmake


.PHONY: wipe # Wipes the persistent cache of fetched dependencies and ccache artifacts
wipe:
	$(CMAKE) -D LEMONS_WIPE_CLEAN=1 -P $(LEMONS_ROOT)/scripts/clean.cmake


.PHONY: format # Runs clang-format over the entire source tree
format:
	$(CMAKE) -P $(LEMONS_ROOT)/scripts/format.cmake
