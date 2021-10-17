SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
	
.DEFAULT_GOAL := help
.PHONY: clean docs format help templates translations uth

#

PYTHON := python

SCRIPTS_DIR := scripts
MODULES := modules
CMAKE_DIR := cmake
CMAKE_FILES := $(shell find $(CMAKE_DIR) -type f -name "*CMakeLists.txt|*.cmake")

TEMP := .out

DOXYGEN_DIR := doxygen
DOXYGEN_BUILD_DIR := $(DOXYGEN_DIR)/build
DOXYGEN_DEPLOY_DIR := $(DOXYGEN_DIR)/doc

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

SOURCE_FILES := $(shell find $(MODULES) -type f -name "$(SOURCE_FILE_PATTERNS)")

TEMPLATES_DIR := default_projects
TEMPLATE_REPOS := $(shell find $(TEMPLATES_DIR) -type d -maxdepth 1 ! -name $(TEMPLATES_DIR))
TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "$(SOURCE_FILE_PATTERNS)")

include $(CMAKE_DIR)/Makefile

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

format: $(TEMP)/format ## Runs clang-format

# Executes the clang-format python script
$(TEMP)/format: $(SCRIPTS_DIR)/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES)
	@echo "Running clang-format..."
	@mkdir -p $(@D)
	@for dir in $(MODULES) $(TEMPLATE_REPOS) ; do $(PYTHON) $< $$dir ; done
	@touch $@

#

### BUILD PROJECT TEMPLATES ###

templates: $(TEMP)/templates ## Builds the template example projects

# Executes the build for the template projects
$(TEMP)/templates: $(TEMPLATES_DIR)/$(BUILD)
	@echo "Building template projects..."
	@mkdir -p $(@D)
	cd $(TEMPLATES_DIR) && $(CMAKE_BUILD_COMMAND)
	@touch $@

# Configures the build for the template projects
$(TEMPLATES_DIR)/$(BUILD): $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(CMAKE_FILES)
	@echo "Configuring cmake..."
	cd $(TEMPLATES_DIR) && $(CMAKE_CONFIGURE_COMMAND)

#

### TRANSLATIONS ###

TRANSLATION_OUTPUT := needed_translations.txt

translations: $(TRANSLATION_OUTPUT) ## Generates a JUCE-style translations file for Lemons

# Executes the translation file generation script
$(TRANSLATION_OUTPUT): $(SCRIPTS_DIR)/generate_translation_file.py $(SOURCE_FILES)
	@echo "Generating Lemons translation file..."
	$(PYTHON) $< $(MODULES) $(TRANSLATION_OUTPUT)

#

### UPDATE GIT SUBMODULES ###

SUBMODULE_COMMAND := git checkout main && git fetch && git pull

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	git fetch && git pull
	git submodule update
	git submodule foreach '$(SUBMODULE_COMMAND)'

#

### CLEAN ###

DEPS_SCRIPT_TEMP_DIR := install_deps/install

clean: ## Cleans the source tree
	@echo "Cleaning Lemons..."
	@rm -rf $(BUILD) $(TEMP) Cache $(TRANSLATION_OUTPUT) $(TEMPLATES_DIR)/$(BUILD) \
		$(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) \
		$(DEPS_SCRIPT_TEMP_DIR)/Brewfile $(DEPS_SCRIPT_TEMP_DIR)/Brewfile.lock.json

#

### HELP ###

help: ## Prints the list of commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

