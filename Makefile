SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: format doxygen translate clean


format: scripts/run_clang_format.py $(SOURCE_FILES)
	python $< modules

doxygen: $(SOURCE_FILES)
	cd doxygen && make

translate: scripts/generate_translation_file.py $(SOURCE_FILES)
	python $<

clean:
	$(RM) -rf Builds 
	cd doxygen && make clean