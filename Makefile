SHELL := /usr/bin/env bash

.PHONY: clean wipe format tests run_tests templates editor docs utils cmake_modules all

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
    CMAKE_CONFIGURE_COMMAND += -D CPM_PrivateSDKs_SOURCE=$(PRIVATE_SDKS)
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

clean:
	cmake -P scripts/clean.cmake

wipe:
	cmake -D LEMONS_WIPE_CLEAN=1 -P scripts/clean.cmake

format:
	cmake -P scripts/format.cmake

#

util/tests/Builds:
	cd util/tests && $(CMAKE_CONFIGURE_COMMAND)

tests: util/tests/Builds
	cd util/tests && $(CMAKE_BUILD_COMMAND)

run_tests: tests
	cd util/tests/Builds && ctest -C Debug

#

util/Templates/Builds:
	cd util/Templates && $(CMAKE_CONFIGURE_COMMAND)

templates: util/Templates/Builds
	cd util/Templates && $(CMAKE_BUILD_COMMAND)

#

util/PluginMetadataEditor/Builds:
	cd util/PluginMetadataEditor && $(CMAKE_CONFIGURE_COMMAND)

editor: util/PluginMetadataEditor/Builds
	cd util/PluginMetadataEditor && $(CMAKE_BUILD_COMMAND)

#

util/doxygen/Builds:
	cd util/doxygen && $(CMAKE_CONFIGURE_COMMAND)

docs: util/doxygen/Builds
	cd util/doxygen && $(CMAKE_BUILD_COMMAND)

#

util/CommandLineUtils/Builds:
	cd util/CommandLineUtils && $(CMAKE_CONFIGURE_COMMAND)

utils: util/CommandLineUtils/Builds
	cd util/CommandLineUtils && $(CMAKE_BUILD_COMMAND)

#

util/cmake/modules/Builds:
	cd util/cmake/modules && $(CMAKE_CONFIGURE_COMMAND)

cmake_modules: util/cmake/modules/Builds
	cd util/cmake/modules && $(CMAKE_BUILD_COMMAND)

#

all: run_tests templates editor docs utils cmake_modules
