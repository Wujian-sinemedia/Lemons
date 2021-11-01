MAKE_UTILS_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

LEMONS_ROOT_DIR := $(abspath $(dir $(dir $(dir $(MAKE_UTILS_DIR)))))

include $(MAKE_UTILS_DIR)/basic_settings.make
include $(MAKE_UTILS_DIR)/targets.make
include $(MAKE_UTILS_DIR)/git.make
include $(MAKE_UTILS_DIR)/system.make
