SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: doxygen format translate clean

#

doxygen: $(SOURCE_FILES)
	cd doxygen && make

format: scripts/run_clang_format.py $(SOURCE_FILES)
	python $< modules

translate: scripts/generate_translation_file.py $(SOURCE_FILES)
	python $< modules lemons_translations.txt

clean:
	$(RM) -rf Builds lemons_translations.txt scripts/install/Brewfile scripts/install/Brewfile.lock.json
	cd doxygen && make clean