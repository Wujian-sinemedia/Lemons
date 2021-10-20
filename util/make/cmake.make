CMAKE_CONFIG_CMD := cmake -B $(BUILD) -G "$(CMAKE_GENERATOR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

ifdef CROSSCOMPILE_IOS
	CMAKE_CONFIG_CMD += -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_CROSSCOMPILING=TRUE -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer"
	# -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=<10 character id>
endif

# This is the configure command actually referenced by makefiles
CMAKE_CONFIGURE_COMMAND := mkdir $(LOGS) && time $(CMAKE_CONFIG_CMD) $(WRITE_CONFIG_LOG)

CMAKE_BUILD_COMMAND := time cmake --build $(BUILD) --config $(BUILD_TYPE) -j $(NUM_CORES)
