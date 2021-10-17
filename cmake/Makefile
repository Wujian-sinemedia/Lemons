ifeq ($(OS),Windows_NT)
	CMAKE_GENERATOR := Visual Studio 16 2019
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		CMAKE_GENERATOR := Unix Makefiles
	else ifeq ($(UNAME_S),Darwin)
		CMAKE_GENERATOR := Xcode
	else 
		$(error Unknown operating system!)
	endif
endif

#

BUILD_TYPE := Debug

BUILD := Builds

CMAKE_CONFIGURE_COMMAND := cmake -B $(BUILD) -G "$(CMAKE_GENERATOR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

CMAKE_BUILD_COMMAND := cmake --build $(BUILD) --config $(BUILD_TYPE)