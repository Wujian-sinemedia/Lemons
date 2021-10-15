SHELL := /bin/bash

SOURCE_FILES := $(shell find modules -type f -name "*.h" | sed 's/ /\\ /g')

.PHONY: config clean

#

config: CMakelists.txt $(SOURCE_FILES)
	cmake -B Builds -G Xcode

build: config $(SOURCE_FILES)
	cmake --build Builds --target All_Build

clean:
	$(RM) -rf Builds