SHELL := /bin/bash

SOURCE_FILES := $(shell find DefaultJuceApp DefaultJucePlugin -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: config format clean

#

config: CMakelists.txt
	cmake -B Builds -G Xcode

build: config
	cmake --build Builds --target ALL_BUILD

format: ../scripts/run_clang_format.py $(SOURCE_FILES)
	python $< DefaultJuceApp
	python $< DefaultJucePlugin

clean:
	$(RM) -rf Builds