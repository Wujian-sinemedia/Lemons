SHELL := /usr/bin/env bash

#

ifeq '$(findstring ;,$(PATH))' ';'
    UNAME := Windows
else
    UNAME := $(shell uname 2>/dev/null || echo Unknown)
    UNAME := $(patsubst CYGWIN%,Cygwin,$(UNAME))
    UNAME := $(patsubst MSYS%,MSYS,$(UNAME))
    UNAME := $(patsubst MINGW%,MSYS,$(UNAME))
endif

ifeq ($(UNAME), Windows)
	CMAKE_GENERATOR := "Visual Studio 16 2019"
else ifeq ($(UNAME), Darwin)
	CMAKE_GENERATOR := Xcode
else
	CMAKE_GENERATOR := Ninja
endif

CMAKE_CONFIGURE_COMMAND := cmake -B Builds -G $(CMAKE_GENERATOR)

ifeq ($(origin PRIVATE_SDKS),undefined)
else
    CMAKE_CONFIGURE_COMMAND += -D CPM_PrivateSDKs_SOURCE="$(PRIVATE_SDKS)"
endif

ifeq ($(UNAME), Linux)
    NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
else ifeq ($(UNAME), Darwin)
    NUMPROC := $(shell sysctl hw.ncpu | awk '{print $$2}')
else 
	NUMPROC := 4
endif

CMAKE_BUILD_COMMAND := cmake --build Builds -j $(NUMPROC)

#

.PHONY: clean # Cleans the Lemons source tree
clean:
	cmake -P scripts/clean.cmake

.PHONY: wipe # Wipes the persistent cache of fetched dependencies and ccache artifacts
wipe:
	cmake -D LEMONS_WIPE_CLEAN=1 -P scripts/clean.cmake

.PHONY: format # Runs clang-format over the entire source tree
format:
	cmake -P scripts/format.cmake

#

util/tests/Builds:
	cd util/tests && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: tests # Builds the tests
tests: util/tests/Builds
	cd util/tests && $(CMAKE_BUILD_COMMAND)

.PHONY: run_tests # Runs all tests
run_tests: tests
	cd util/tests/Builds && ctest -C Debug

#

util/Templates/Builds:
	cd util/Templates && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: templates # Builds the project templates
templates: util/Templates/Builds
	cd util/Templates && $(CMAKE_BUILD_COMMAND)

#

util/PluginMetadataEditor/Builds:
	cd util/PluginMetadataEditor && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: editor # Builds the plugin metadata editor
editor: util/PluginMetadataEditor/Builds
	cd util/PluginMetadataEditor && $(CMAKE_BUILD_COMMAND)

#

util/doxygen/Builds:
	cd util/doxygen && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: docs # Builds the documentation
docs: util/doxygen/Builds
	cd util/doxygen && $(CMAKE_BUILD_COMMAND)

#

util/CommandLineUtils/Builds:
	cd util/CommandLineUtils && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: utils # Builds the command line utilities
utils: util/CommandLineUtils/Builds
	cd util/CommandLineUtils && $(CMAKE_BUILD_COMMAND)

#

util/cmake/modules/Builds:
	cd util/cmake/modules && $(CMAKE_CONFIGURE_COMMAND)

.PHONY: cmake_modules # Builds the cmake modules
cmake_modules: util/cmake/modules/Builds
	cd util/cmake/modules && $(CMAKE_BUILD_COMMAND)

.PHONY: install_cmake_modules # Builds and installs the cmake modules
install_cmake_modules: cmake_modules
	cd util/cmake/modules && sudo cmake --install Builds

#

.PHONY: all # Builds and runs all CI
all: run_tests templates editor docs utils install_cmake_modules

#

.PHONY: help # Print this message
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20 | sort
