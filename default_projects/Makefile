SHELL := /bin/bash

SOURCE_FILES := $(shell find DefaultJuceApp DefaultJucePlugin -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: format translate clean

#

config: Builds/CMakeCache.txt
	cmake -B Builds -G Xcode

build: config $(SOURCE_FILES)
	cmake --build Builds --target ALL_BUILD

format: ../scripts/run_clang_format.py $(SOURCE_FILES)
	python $< DefaultJuceApp
	python $< DefaultJucePlugin

translate: ../scripts/generate_translation_file.py $(SOURCE_FILES)
	python $< DefaultJuceApp app_translations.txt
	python $< DefaultJucePlugin plugin_translations.txt

clean:
	$(RM) -rf Builds app_translations.txt plugin_translations.txt