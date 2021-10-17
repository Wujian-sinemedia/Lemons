#!/usr/bin/env bash

# Installs a list of dependencies using Chocolatey

set -euo pipefail

# TO DO: Check if Chocolatey is installed, and install it if need be...?


process_line() {
	local -r line=$(echo "$1" | sed 's/;//')

	# AFAIK Chocolatey doesn't have packages by these names
	for other in "clang-format" "doxygen"
	do
		if [ "$line" = "$other" ]; then
			return
		fi
	done

	choco install "$line"
}


os_install_func() {

	cat "$1" | while read line || [[ -n "$line" ]];
	do
		process_line "$line"
	done
}
