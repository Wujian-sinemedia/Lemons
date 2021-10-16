SHELL := /bin/bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
.DEFAULT_GOAL := help

.PHONY: clean help uth

SOURCE_FILES := $(shell find modules -type f -name "*.h|*.cpp|*CMakeLists.txt")

CMAKE_FILES := $(shell find cmake -type f -name "*CMakeLists.txt|*.cmake")

#

TEMPLATES_DIR := default_projects

TEMPLATE_REPOS := $(TEMPLATES_DIR)/DefaultJuceApp $(TEMPLATES_DIR)/DefaultJucePlugin

TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "*.h|*.cpp|*CMakeLists.txt")

#

CMAKE_GENERATOR := Xcode

#

defaults: .out/defaults ## Builds the template example projects

.out/defaults: $(TEMPLATES_DIR)/Builds
	@echo "Building template projects..."
	@mkdir -p $(@D)
	cd $(TEMPLATES_DIR) && cmake --build Builds --target ALL_BUILD
	@touch $@

# Configures the build for the template projects
$(TEMPLATES_DIR)/Builds: $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(CMAKE_FILES)
	@echo "Configuring cmake..."
	cd $(TEMPLATES_DIR) && cmake -B Builds -G $(CMAKE_GENERATOR)

#

format: .out/format ## Runs clang-format

.out/format: scripts/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES)
	@echo "Running clang-format..."
	@mkdir -p $(@D)

	for dir in modules $(TEMPLATE_REPOS) ; do \
		python $< $$dir ; \
	done

	@touch $@

SUBMODULE_COMMAND := git checkout main && git fetch && git pull && git submodule update

uth: ## Updates all git submodules to head
	@echo "Updating git submodules..."
	git fetch && git pull
	git submodule update
	git submodule foreach '$(SUBMODULE_COMMAND)'

#

DOXYGEN_BUILD_DIR := doxygen/build
DOXYGEN_DEPLOY_DIR := doxygen/doc

docs: $(DOXYGEN_DEPLOY_DIR)/index.html ## Builds the documentation

$(DOXYGEN_DEPLOY_DIR)/index.html: $(DOXYGEN_BUILD_DIR)/lemons_modules.dox doxygen/Doxyfile doxygen/DoxygenLayout.xml doxygen/main_page.md cmake/README.md
	@echo "Building documentation..."
	cd doxygen && doxygen

# Parses the module tree to create doxygen's build tree, and the module subgroups' descriptions file
$(DOXYGEN_BUILD_DIR)/lemons_modules.dox: doxygen/process_source_files.py $(SOURCE_FILES)
	python $<

#

clean: ## Cleans the source tree
	$(RM) -rf Builds $(TEMPLATES_DIR)/Builds $(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) scripts/install/Brewfile scripts/install/Brewfile.lock.json

help: ## Prints the list of commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

