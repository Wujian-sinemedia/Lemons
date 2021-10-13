SHELL := /bin/bash

SOURCE_FILES := $(shell find ../modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: clean

doc/index.html: build/lemons_modules.dox Doxyfile
	doxygen

build/lemons_modules.dox: process_source_files.py $(SOURCE_FILES)
	python $<

clean:
	$(RM) -rf build doc Lemons