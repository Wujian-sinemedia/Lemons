SHELL := /bin/bash

.PHONY: defaults doxygen test format translate uth clean

#

defaults:
	cd default_projects && make build

doxygen: 
	cd doxygen && make

format: scripts/run_clang_format.py 
	python $< modules
	cd default_projects && make format

test: defaults

translate: scripts/generate_translation_file.py 
	python $< modules lemons_translations.txt
	cd default_projects && make translate

uth:
	git fetch && git pull
	git submodule update
	git submodule foreach 'git checkout main && git fetch && git pull && git submodule update'

clean:
	$(RM) -rf Builds lemons_translations.txt scripts/install/Brewfile scripts/install/Brewfile.lock.json
	cd doxygen && make clean
	cd default_projects && make clean