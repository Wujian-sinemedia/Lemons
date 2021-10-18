SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
	
.DEFAULT_GOAL := help
.PHONY: clean docs format help templates translations uth

SCRIPTS_DIR := scripts
MODULES := modules
UTIL_DIR := util
MAKE_DIR := $(UTIL_DIR)/make
CMAKE_DIR := $(UTIL_DIR)/cmake
DOXYGEN_DIR := $(UTIL_DIR)/doxygen
DOXYGEN_BUILD_DIR := $(DOXYGEN_DIR)/build
DOXYGEN_DEPLOY_DIR := $(DOXYGEN_DIR)/doc

include $(MAKE_DIR)/basic_settings.make
include $(MAKE_DIR)/cmake.make

TEMPLATES_DIR := templates
TEMPLATE_REPOS := $(shell find $(TEMPLATES_DIR) -type d -maxdepth 1 ! -name $(TEMPLATES_DIR))
TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "$(SOURCE_FILE_PATTERNS)")
SOURCE_FILES := $(shell find $(MODULES) -type f -name "$(SOURCE_FILE_PATTERNS)")

#

### DOCS ###

MODULE_DOC_OUTPUT := $(DOXYGEN_BUILD_DIR)/lemons_modules.dox
HTML_DOC_OUTPUT := $(DOXYGEN_DEPLOY_DIR)/index.html

docs: $(HTML_DOC_OUTPUT) ## Builds the documentation

# Executes doxygen to build the documentation
$(HTML_DOC_OUTPUT): $(MODULE_DOC_OUTPUT) $(shell find $(DOXYGEN_DIR) -type f -maxdepth 1 ! -name *process_source_files.py)
	@echo "Building documentation..."
	cd $(DOXYGEN_DIR) && doxygen

# Parses the module tree to create doxygen's build tree, and the module subgroups' descriptions file
$(MODULE_DOC_OUTPUT): $(DOXYGEN_DIR)/process_source_files.py $(CMAKE_DIR)/README.md $(SOURCE_FILES)
	$(PYTHON) $<

#

### CLANG FORMAT ###

format: $(SCRIPTS_DIR)/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) ## Runs clang-format
	@echo "Running clang-format..."
	@for dir in $(MODULES) $(TEMPLATE_REPOS) ; do $(PYTHON) $< $$dir ; done

#

### BUILD PROJECT TEMPLATES ###

templates: $(TEMPLATES_DIR)/$(BUILD) ## Builds the template example projects
	@echo "Building template projects..."
	cd $(TEMPLATES_DIR) && $(CMAKE_BUILD_COMMAND)

# Configures the build for the template projects
$(TEMPLATES_DIR)/$(BUILD): $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(shell find $(CMAKE_DIR) -type f -name "$(CMAKE_FILE_PATTERNS)")
	@echo "Configuring cmake..."
	cd $(TEMPLATES_DIR) && $(CMAKE_CONFIGURE_COMMAND)

#

### TRANSLATIONS ###

translations: $(TRANSLATION_OUTPUT) ## Generates a JUCE-style translations file for Lemons

# Executes the translation file generation script
$(TRANSLATION_OUTPUT): $(SCRIPTS_DIR)/generate_translation_file.py $(SOURCE_FILES)
	@echo "Generating Lemons translation file..."
	$(PYTHON) $< $(MODULES) $(TRANSLATION_OUTPUT)

#

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	$(GIT_UTH)

#

DEPS_SCRIPT_TEMP_DIR := $(UTIL_DIR)/install_deps/install

clean: ## Cleans the source tree
	@echo "Cleaning Lemons..."
	@rm -rf $(BUILD) $(CACHE) $(TRANSLATION_OUTPUT) $(TEMPLATES_DIR)/$(BUILD) .github/docs \
		$(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) \
		$(DEPS_SCRIPT_TEMP_DIR)/Brewfile $(DEPS_SCRIPT_TEMP_DIR)/Brewfile.lock.json


help: ## Prints the list of commands
	@$(PRINT_HELP_LIST)
