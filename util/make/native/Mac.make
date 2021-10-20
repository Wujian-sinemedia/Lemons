CMAKE_GENERATOR := Xcode

NUM_CORES := $(shell sysctl hw.ncpu | awk '{print $$2}')