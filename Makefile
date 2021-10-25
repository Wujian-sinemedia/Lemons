SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:

.DEFAULT_GOAL := help

THIS_FILE := $(lastword $(MAKEFILE_LIST))

SCRIPTS_DIR := scripts
MODULES := modules
UTIL_DIR := util
MAKE_DIR := $(UTIL_DIR)/make
CMAKE_DIR := $(UTIL_DIR)/cmake
DOXYGEN_DIR := $(UTIL_DIR)/doxygen
DOXYGEN_BUILD_DIR := $(DOXYGEN_DIR)/build
DOXYGEN_DEPLOY_DIR := $(DOXYGEN_DIR)/doc

include $(MAKE_DIR)/Makefile

.PHONY: $(ALL_PHONY_TARGETS)

SOURCE_FILES := $(shell find $(MODULES) -type f -name "$(SOURCE_FILE_PATTERNS)")


#####  DOCS  #####

MODULE_DOC_OUTPUT := $(DOXYGEN_BUILD_DIR)/lemons_modules.dox
HTML_DOC_OUTPUT := $(DOXYGEN_DEPLOY_DIR)/index.html

docs: $(HTML_DOC_OUTPUT) ## Builds the documentation

DOC_SCRIPT_NAME := process_source_files.py

$(HTML_DOC_OUTPUT): $(MODULE_DOC_OUTPUT) $(shell find $(DOXYGEN_DIR) -type f -maxdepth 1 ! -name *$(DOC_SCRIPT_NAME))
	@echo "Building documentation..."
	cd $(DOXYGEN_DIR) && doxygen

$(MODULE_DOC_OUTPUT): $(DOXYGEN_DIR)/$(DOC_SCRIPT_NAME) $(SOURCE_FILES)
	$(PYTHON) $<


###  UTILITIES  ###

propogate: $(SCRIPTS_DIR)/propogate_config_files.py ## Propogates all configuration files to the template repos
	@echo "Propogating configuration files to template repos..."
	@for dir in $(TEMPLATE_REPOS) ; do $(PYTHON) $< $$dir ; done

format: $(SCRIPTS_DIR)/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) .clang-format ## Runs clang-format
	@echo "Running clang-format..."
	@for dir in $(MODULES) $(TEMPLATE_REPOS) ; do $(PYTHON) $< $$dir ; done

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	@$(GIT_UTH)

#

DEPS_SCRIPT_TEMP_DIR := $(UTIL_DIR)/install_deps

clean: ## Cleans the source tree
	@echo "Cleaning Lemons..."
	@$(RM) $(BUILD) $(LOGS) .github/docs Lemons_translations.txt \
		$(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) \
		$(DEPS_SCRIPT_TEMP_DIR)/Brewfile $(DEPS_SCRIPT_TEMP_DIR)/Brewfile.lock.json

wipe: clean ## Cleans everything, and busts the CPM cache
	@echo "Wiping Lemons cache..."
	@$(RM) $(CACHE)

help: ## Prints the list of commands
	@$(PRINT_HELP_LIST)
