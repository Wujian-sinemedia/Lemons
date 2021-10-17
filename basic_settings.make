PYTHON := python

TEMP := .out
CACHE := Cache

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

TRANSLATION_OUTPUT := needed_translations.txt

#

PRINT_HELP_LIST := grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

GIT_SUBMODULE_UTH_COMMAND := git checkout main && git fetch && git pull

GIT_UTH := git fetch && git pull && git submodule update && git submodule foreach '$(GIT_SUBMODULE_UTH_COMMAND)'
