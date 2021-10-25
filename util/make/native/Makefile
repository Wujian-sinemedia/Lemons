NATIVE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

ifeq ($(OS),Windows_NT)
	include $(NATIVE_DIR)/Windows.make
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		include $(NATIVE_DIR)/Linux.make
	else ifeq ($(UNAME_S),Darwin)
		include $(NATIVE_DIR)/Mac.make
	else 
		$(error Unknown operating system!)
	endif
endif

ifneq ($J,)
	NUM_CORES := $J
endif