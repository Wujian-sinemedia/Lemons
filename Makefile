SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: clean translate doxygen


doxygen: $(SOURCE_FILES)
	cd doxygen && make

translate: scripts/generate_translation_file.py $(SOURCE_FILES)
	python $<

clean:
	$(RM) -rf Builds 
	cd doxygen && make clean