SHELL := /bin/bash

.PHONY: config clean

#

config: CMakelists.txt
	cmake -B Builds -G Xcode

build: config
	cmake --build Builds --target All_Build

clean:
	$(RM) -rf Builds