PYTHON := python3

RM := rm -rf

#

BUILD_TYPE := Debug

CACHE := Cache

BUILD := Builds

TRANSLATION_OUTPUT := needed_translations.txt

LOGS := logs
CONFIG_LOG_FILE := $(LOGS)/config.log
BUILD_LOG_FILE := $(LOGS)/build.log

SOURCE_FILE_PATTERNS := *.h|*.cpp|*CMakeLists.txt

CMAKE_FILE_PATTERNS := *CMakeLists.txt|*.cmake

GIT_BRANCH_NAME := main
