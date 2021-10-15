SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: doxygen format translate uth clean

#

doxygen: $(SOURCE_FILES)
	cd doxygen && make

format: scripts/run_clang_format.py $(SOURCE_FILES)
	python $< modules

translate: scripts/generate_translation_file.py $(SOURCE_FILES)
	python $< modules lemons_translations.txt

uth:
	git fetch && git pull
	git submodule update
	git submodule foreach 'git checkout main && git fetch && git pull && git submodule update'

clean:
	$(RM) -rf Builds lemons_translations.txt scripts/install/Brewfile scripts/install/Brewfile.lock.json
	cd doxygen && make clean
	cd default_projects && make clean