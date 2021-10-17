SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := help
.PHONY: clean defaults docs format help uth


#
#
# Input configuration

BUILD_TYPE := Debug


#
#
# Subdirs and files

PYTHON := python

SCRIPTS_DIR := scripts

CMAKE_DIR := cmake

TEMP := .out

BUILD := Builds

MODULES := modules

TEMPLATES_DIR := default_projects
TEMPLATE_REPOS := $(shell find $(TEMPLATES_DIR) -type d)

DOXYGEN_DIR := doxygen
DOXYGEN_BUILD_DIR := $(DOXYGEN_DIR)/build
DOXYGEN_DEPLOY_DIR := $(DOXYGEN_DIR)/doc

CMAKE_FILES := $(shell find $(CMAKE_DIR) -type f -name "*CMakeLists.txt|*.cmake")

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

SOURCE_FILES := $(shell find $(MODULES) -type f -name "$(SOURCE_FILE_PATTERNS)")

TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "$(SOURCE_FILE_PATTERNS)")


#
#
# Detect appropriate CMake generator for OS

ifeq ($(OS),Windows_NT)
	CMAKE_GENERATOR := \"Visual Studio 16 2019\"
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		CMAKE_GENERATOR := \"Unix Makefiles\"
	else ifeq ($(UNAME_S),Darwin)
		CMAKE_GENERATOR := Xcode
	else 
		$(error Unknown operating system!)
	endif
endif


#
#
# Rules to build the template projects

DEFAUT_SENTINEL := $(TEMP)/defaults

defaults: $(DEFAUT_SENTINEL) ## Builds the template example projects

# Executes the build for the template projects
$(DEFAUT_SENTINEL): $(TEMPLATES_DIR)/$(BUILD)
	@echo "Building template projects..."
	@mkdir -p $(@D)
	cd $(TEMPLATES_DIR) && cmake --build $(BUILD) --target ALL_BUILD --config $(BUILD_TYPE)
	@touch $@

# Configures the build for the template projects
$(TEMPLATES_DIR)/$(BUILD): $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(CMAKE_FILES)
	@echo "Configuring cmake..."
	cd $(TEMPLATES_DIR) && cmake -B $(BUILD) -G $(CMAKE_GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)


#
#
# Rules to run clang-format

FORMAT_SENTINEL := $(TEMP)/format

format: $(FORMAT_SENTINEL) ## Runs clang-format

# Executes the clang-format python script
$(FORMAT_SENTINEL): $(SCRIPTS_DIR)/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES)
	@echo "Running clang-format..."
	@mkdir -p $(@D)

	for dir in $(MODULES) $(TEMPLATE_REPOS) ; do \
		$(PYTHON) $< $$dir ; \
	done

	@touch $@


#
#
# Rules to update git submodules

SUBMODULE_COMMAND := git checkout main && git fetch && git pull

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	git fetch && git pull
	git submodule update
	git submodule foreach '$(SUBMODULE_COMMAND)'


#
#
# Rules to build the documentation

docs: $(DOXYGEN_DEPLOY_DIR)/index.html ## Builds the documentation

# Executes doxygen to build the documentation
$(DOXYGEN_DEPLOY_DIR)/index.html: $(DOXYGEN_BUILD_DIR)/lemons_modules.dox $(DOXYGEN_DIR)/Doxyfile $(DOXYGEN_DIR)/DoxygenLayout.xml $(DOXYGEN_DIR)/main_page.md
	@echo "Building documentation..."
	cd $(DOXYGEN_DIR) && doxygen

# Parses the module tree to create doxygen's build tree, and the module subgroups' descriptions file
$(DOXYGEN_BUILD_DIR)/lemons_modules.dox: $(DOXYGEN_DIR)/process_source_files.py $(CMAKE_DIR)/README.md $(SOURCE_FILES)
	$(PYTHON) $<


#
#
# Standard utilities

DEPS_SCRIPT_TEMP_DIR := install_deps/install

clean: ## Cleans the source tree
	rm -rf $(BUILD) $(TEMP) $(TEMPLATES_DIR)/$(BUILD) $(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) $(DEPS_SCRIPT_TEMP_DIR)/Brewfile $(DEPS_SCRIPT_TEMP_DIR)/Brewfile.lock.json

help: ## Prints the list of commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

