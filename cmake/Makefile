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

ifdef CROSSCOMPILE_IOS
	CMAKE_CONFIGURE_COMMAND += " -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_CROSSCOMPILING=TRUE -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY=\"iPhone Developer\""
	# CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=<10 character id>
endif

CMAKE_BUILD_COMMAND := cmake --build $(BUILD) --config $(BUILD_TYPE)