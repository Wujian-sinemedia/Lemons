SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: clean translate doxygen

doxygen: $(SOURCE_FILES)
	cd doxygen && make

translate: $(SOURCE_FILES)
	python $<

clean:
	cd doxygen && make clean