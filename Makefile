SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := help
.PHONY: clean defaults docs format help uth

#

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

SOURCE_FILES := $(shell find modules -type f -name "$(SOURCE_FILE_PATTERNS)")

CMAKE_FILES := $(shell find cmake -type f -name "*CMakeLists.txt|*.cmake")

TEMPLATES_DIR := default_projects

TEMPLATE_REPOS := $(TEMPLATES_DIR)/DefaultJuceApp $(TEMPLATES_DIR)/DefaultJucePlugin

TEMPLATE_PROJECT_FILES := $(shell find $(TEMPLATE_REPOS) -type f -name "$(SOURCE_FILE_PATTERNS)")

BUILD_TYPE := Debug

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

defaults: .out/defaults ## Builds the template example projects

.out/defaults: $(TEMPLATES_DIR)/Builds
	@echo "Building template projects..."
	@mkdir -p $(@D)
	cd $(TEMPLATES_DIR) && cmake --build Builds --target ALL_BUILD --config $(BUILD_TYPE)
	@touch $@

# Configures the build for the template projects
$(TEMPLATES_DIR)/Builds: $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(CMAKE_FILES)
	@echo "Configuring cmake..."
	cd $(TEMPLATES_DIR) && cmake -B Builds -G $(CMAKE_GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

#

format: .out/format ## Runs clang-format

.out/format: scripts/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES)
	@echo "Running clang-format..."
	@mkdir -p $(@D)

	for dir in modules $(TEMPLATE_REPOS) ; do \
		python $< $$dir ; \
	done

	@touch $@

#

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

$(DOXYGEN_DEPLOY_DIR)/index.html: $(DOXYGEN_BUILD_DIR)/lemons_modules.dox doxygen/Doxyfile doxygen/DoxygenLayout.xml doxygen/main_page.md
	@echo "Building documentation..."
	cd doxygen && doxygen

# Parses the module tree to create doxygen's build tree, and the module subgroups' descriptions file
$(DOXYGEN_BUILD_DIR)/lemons_modules.dox: doxygen/process_source_files.py $(SOURCE_FILES) cmake/README.md
	python $<

#

clean: ## Cleans the source tree
	rm -rf Builds $(TEMPLATES_DIR)/Builds $(DOXYGEN_BUILD_DIR) $(DOXYGEN_DEPLOY_DIR) scripts/install/Brewfile scripts/install/Brewfile.lock.json

help: ## Prints the list of commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

