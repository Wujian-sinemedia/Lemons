ifeq ($(OS),Windows_NT)
	NUM_CORES := $(NUMBER_OF_PROCESSORS)
	CMAKE_GENERATOR := Visual Studio 16 2019
else
	ifeq ($(shell uname -s),Darwin)
			NUM_CORES := $(shell sysctl hw.ncpu | awk '{print $$2}')
			CMAKE_GENERATOR := Xcode
		else
			NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)
			CMAKE_GENERATOR := Ninja
		endif
endif

ifneq ($J,)
	NUM_CORES := $J
endif
