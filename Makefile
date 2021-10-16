SHELL := /bin/bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
.DEFAULT_GOAL := help

.PHONY: clean uth

SOURCE_FILES := $(shell find modules -type f -name "*.h|*.cpp|*CMakeLists.txt" | sed 's/ /\\ /g')

CMAKE_FILES := $(shell find cmake -type f -name "*CMakeLists.txt|*.cmake" | sed 's/ /\\ /g')

TEMPLATE_PROJECT_FILES := $(shell find default_projects/DefaultJuceApp default_projects/DefaultJucePlugin -type f -name "*.h|*.cpp|*CMakeLists.txt" | sed 's/ /\\ /g')

TEMPLATE_PROJECT_CMAKE_OUT := default_projects/Builds/CMakeCache.txt

CMAKE_GENERATOR := Xcode

#

defaults: .out/defaults ## Builds the template example projects

.out/defaults: $(TEMPLATE_PROJECT_CMAKE_OUT)
	@echo "Building template projects..."
	mkdir -p $(@D)
	cd default_projects && cmake --build Builds --target ALL_BUILD
	touch $@

# Configures the build for the template projects
$(TEMPLATE_PROJECT_CMAKE_OUT): $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES) $(CMAKE_FILES)
	cd default_projects && cmake -B Builds -G $(CMAKE_GENERATOR)

#

format: .out/format ## Runs clang-format

.out/format: scripts/run_clang_format.py $(TEMPLATE_PROJECT_FILES) $(SOURCE_FILES)
	@echo "Running clang-format..."
	mkdir -p $(@D)

	for dir in modules default_projects/DefaultJuceApp default_projects/DefaultJucePlugin ; do \
		python $< $$dir 2>&1 >/dev/null ; \
	done

	touch $@

uth: ## Updates all git submodules to head
	git fetch && git pull
	git submodule update
	git submodule foreach 'git checkout main && git fetch && git pull && git submodule update'

#

docs: doxygen/doc/index.html ## Builds the documentation

doxygen/doc/index.html: doxygen/build/lemons_modules.dox doxygen/Doxyfile doxygen/DoxygenLayout.xml doxygen/main_page.md cmake/README.md
	cd doxygen && doxygen

# Parses the module tree to create doxygen's build tree, and the module subgroups' descriptions file
doxygen/build/lemons_modules.dox: doxygen/process_source_files.py $(SOURCE_FILES)
	python $<

#

clean: ## Cleans the source tree
	$(RM) -rf Builds default_projects/Builds scripts/install/Brewfile scripts/install/Brewfile.lock.json doxygen/build doxygen/doc

# Prints a list of commands
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

