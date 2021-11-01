SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:

.DEFAULT_GOAL := help

THIS_FILE := $(lastword $(MAKEFILE_LIST))

include util/make/Makefile

.PHONY: $(ALL_PHONY_TARGETS) build_tests


#####  DOCS  #####

docs: Builds/docs ## Builds the Doxygen documentation
	cmake --build --preset docs -j $(NUM_CORES)

Builds/docs:
	cmake --preset docs -G $(CMAKE_GENERATOR)


#####  TEMPLATES  #####

templates: Builds/templates ## Builds the Lemons template projects
	cmake --build --preset templates -j $(NUM_CORES)

Builds/templates:
	cmake --preset templates -G $(CMAKE_GENERATOR)


#####  TESTS  #####

tests: build_tests ## Builds and runs the Lemons unit tests
	ctest --preset all

build_tests: Builds/tests
	cmake --build --preset tests -j $(NUM_CORES)

Builds/tests:
	cmake --preset tests -G $(CMAKE_GENERATOR)


###  UTILITIES  ###

# TEMPLATE_REPOS := $(shell find templates -type d -maxdepth 1 ! -name templates)

# propogate: ## Propogates all configuration files to the template repos
# 	@echo "Propogating configuration files to template repos..."
# 	@for dir in $(TEMPLATE_REPOS) ; do $(PYTHON) scripts/project_config/propogate_config_files.py $$dir ; done

# format: scripts/run_clang_format.py .clang-format ## Runs clang-format
# 	@echo "Running clang-format..."
# 	@for dir in modules $(TEMPLATE_REPOS) ; do $(PYTHON) $< $$dir ; done

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	@$(GIT_UTH)

#

DEPS_SCRIPT_TEMP_DIR := util/install_deps

clean: ## Cleans the source tree
	@echo "Cleaning Lemons..."
	@$(RM) Builds logs .github/docs \
		$(DEPS_SCRIPT_TEMP_DIR)/Brewfile $(DEPS_SCRIPT_TEMP_DIR)/Brewfile.lock.json

wipe: clean ## Cleans the source tree and dumps the cache
	@echo "Wiping Lemons cache..."
	@$(RM) Cache

help: ## Prints the list of commands
	@$(PRINT_HELP_LIST)
