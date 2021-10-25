PYTHON := python3

RM := rm -rf

#

BUILD_TYPE := Debug

CACHE := Cache

BUILD := Builds

SOURCE := Source

TRANSLATIONS := translations

TRANSLATION_FILE_TEMPLATE := $(TRANSLATIONS)/needed_translations.txt

LOGS := logs

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

CMAKE_FILE_PATTERNS := *CMakeLists.txt|*.cmake

GIT_BRANCH_NAME := main
