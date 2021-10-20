# Prints a list of all targets commented with '##'
PRINT_HELP_LIST := grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_FILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Contains a list of all targets commented with '##'. These are usually phony targets, so this can be useful for automatically assigning them all as phony.
ALL_PHONY_TARGETS := $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_FILE) | sort | sed 's/:.*/\ /' | tr '\n' ' ')
