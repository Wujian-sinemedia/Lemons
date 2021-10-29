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

include $(MAKE_DIR)/Makefile

.PHONY: $(ALL_PHONY_TARGETS)

TEMPLATES_DIR := templates
TEMPLATE_REPOS := $(shell find $(TEMPLATES_DIR) -type d -maxdepth 1 ! -name $(TEMPLATES_DIR))
TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "$(SOURCE_FILE_PATTERNS)")
SOURCE_FILES := $(shell find $(MODULES) -type f -name "$(SOURCE_FILE_PATTERNS)")


#####  DOCS  #####

docs: config_docs ## Builds the Doxygen documentation
	cmake --build --preset docs

config_docs:
	cmake --preset docs

.PHONY: config_docs


###  UTILITIES  ###

propogate: $(SCRIPTS_DIR)/project_config/propogate_config_files.py ## Propogates all configuration files to the template repos
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

help: ## Prints the list of commands
	@$(PRINT_HELP_LIST)
