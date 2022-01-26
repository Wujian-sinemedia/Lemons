SHELL = /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

CONFIGS = Debug Release

CMAKE = cmake
CTEST = ctest
PRECOMMIT = pre-commit

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

BUILDS = Builds

#

override LEMONS_ROOT := $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

override THIS_MAKEFILE := $(LEMONS_ROOT)/Makefile

override make_build_dir = $(LEMONS_ROOT)/util/$(1)/$(BUILDS)

override cmake_config = cd $(dir $(1)) && $(CMAKE) -B $(1) -G "$(CMAKE_GENERATOR)" --log-level=DEBUG

override cmake_build_configuration = echo "Building $(2) configuration..."; $(CMAKE) --build $(1) -j $(NUM_CORES) --config $(2)

override cmake_build = $(foreach config,$(CONFIGS),$(call cmake_build_configuration,$(1),$(config));)

override ctest_test_config = echo "Testing $(1) configuration..."; cd $(call make_build_dir,tests) && $(CTEST) -C $(1) --verbose --repeat until-pass:3

#

help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

$(call make_build_dir,tests):
	@$(call cmake_config,$@)

tests: $(call make_build_dir,tests)  ## Builds the tests
	@$(call cmake_build,$<)

run_tests: tests  ## Runs all tests
	@$(foreach config,$(CONFIGS),$(call ctest_test_config,$(config));)

#

$(call make_build_dir,Templates):
	@$(call cmake_config,$@)

templates: $(call make_build_dir,Templates)  ## Builds the project templates
	@$(call cmake_build,$<)

#

$(call make_build_dir,PluginMetadataEditor):
	@$(call cmake_config,$@)

editor: $(call make_build_dir,PluginMetadataEditor)  ## Builds the plugin metadata editor
	@$(call cmake_build,$<)

#

$(call make_build_dir,CommandLineUtils):
	@$(call cmake_config,$@)

utils: $(call make_build_dir,CommandLineUtils)  ## Builds the command line utilities
	@$(call cmake_build,$<)

#

docs:  ## Builds the documentation
	$(eval DOXYGEN_DIR = $(LEMONS_ROOT)/util/doxygen)
	@cd $(DOXYGEN_DIR) && $(CMAKE) -B $(BUILDS)
	@cd $(DOXYGEN_DIR) && $(CMAKE) --build $(BUILDS) -j $(NUM_CORES)

#

all: tests templates editor utils docs  ## Builds everything

#

SCRIPTS_DIR = $(LEMONS_ROOT)/scripts

clean:  ## Cleans the Lemons source tree
	$(CMAKE) -P $(SCRIPTS_DIR)/clean.cmake
	$(PRECOMMIT) gc


wipe:  ## Wipes the persistent cache of fetched dependencies and ccache artifacts
	$(CMAKE) -D LEMONS_WIPE_CLEAN=1 -P $(SCRIPTS_DIR)/clean.cmake
	$(PRECOMMIT) clean


init:  ## Initializes the Lemons workspace and installs all dependencies
	@chmod +x $(SCRIPTS_DIR)/clean.cmake $(SCRIPTS_DIR)/install_deps.cmake
	$(CMAKE) -P $(SCRIPTS_DIR)/install_deps.cmake
	@cd $(LEMONS_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite
	@cd $(LEMONS_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg


pc:  ## Runs all pre-commit hooks over all files
	@cd $(LEMONS_ROOT) && $(PRECOMMIT) run --all-files


.PHONY: $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sed 's/:.*/\ /' | tr '\n' ' ')
