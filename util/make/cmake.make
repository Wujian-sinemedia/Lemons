ifeq ($(OS),Windows_NT)
	CMAKE_GENERATOR := Visual Studio 16 2019
	NUM_CORES := $(NUMBER_OF_PROCESSORS)
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		CMAKE_GENERATOR := Ninja
		NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)
	else ifeq ($(UNAME_S),Darwin)
		CMAKE_GENERATOR := Xcode
		NUM_CORES := $(shell sysctl hw.ncpu | awk '{print $$2}')
	else 
		$(error Unknown operating system!)
	endif
endif

ifneq ($J,)
	NUM_CORES := $J
endif

#

BUILD_TYPE := Debug

BUILD := Builds

CONFIG_LOG_FILE := config.log
BUILD_LOG_FILE := build.log

#

CMAKE_CONFIG_CMD := cmake -B $(BUILD) -G "$(CMAKE_GENERATOR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

ifdef CROSSCOMPILE_IOS
	CMAKE_CONFIG_CMD += -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_CROSSCOMPILING=TRUE -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer"
	# CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=<10 character id>
endif

# This is the command actually referenced by makefiles
CMAKE_CONFIGURE_COMMAND := time $(CMAKE_CONFIG_CMD) | tee $(CONFIG_LOG_FILE)

#

CMAKE_BUILD_COMMAND := cmake --build $(BUILD) --config $(BUILD_TYPE) -j $(NUM_CORES)
