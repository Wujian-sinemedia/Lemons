CMAKE_GENERATOR := Ninja

NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)