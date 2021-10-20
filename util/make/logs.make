CONFIG_LOG_FILE := $(LOGS)/config.log
BUILD_LOG_FILE := $(LOGS)/build.log

CONFIG_ERRORS_LOG_FILE := $(LOGS)/config_errors.log
BUILD_ERRORS_LOG_FILE := $(LOGS)/build_errors.log

#

POST_PROCESS_LOG := $(PYTHON) $(MAKE_UTILS_DIR)/process_logs.py $(shell pwd)

#
# These are the public commands

WRITE_CONFIG_LOG := 2>&1 | tee $(CONFIG_LOG_FILE) && $(POST_PROCESS_LOG) $(CONFIG_LOG_FILE) $(CONFIG_ERRORS_LOG_FILE)

WRITE_BUILD_LOG := 2>&1 | tee $(BUILD_LOG_FILE) && $(POST_PROCESS_LOG) $(BUILD_LOG_FILE) $(BUILD_ERRORS_LOG_FILE)
