PYTHON := python3

RM := rm -rf

#

CACHE := Cache

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

CMAKE_FILE_PATTERNS := *CMakeLists.txt|*.cmake

TRANSLATION_OUTPUT := needed_translations.txt

#

# Prints a list of all targets commented with '##'
PRINT_HELP_LIST := grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_FILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Contains a list of all targets commented with '##'. These are usually phony targets, so this can be useful for automatically assigning them all as phony.
ALL_PHONY_TARGETS := $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_FILE) | sort | sed 's/:.*/\ /' | tr '\n' ' ')

#

GIT_BRANCH_NAME := main

GIT_SUBMODULE_UTH_COMMAND := git checkout $(GIT_BRANCH_NAME) && git fetch && git pull

GIT_UTH := git fetch && git pull && git submodule update && git submodule foreach '$(GIT_SUBMODULE_UTH_COMMAND)'
