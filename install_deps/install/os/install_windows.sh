#!/usr/bin/env bash

# Installs a list of dependencies using Chocolatey

set -euo pipefail


process_line() {
	local -r line=$(echo "$1" | sed 's/;//')

	for other in "clang-format" "doxygen"
	do
		if [ "$line" = "$other" ]; then
			return
		fi
	done

	echo "line : \"$line\""

	choco install "$line"
}


os_install_func() {

	local -r DEPS_LIST="$1"

	cat "$DEPS_LIST" | while read line || [[ -n "$line" ]];
	do
		process_line "$line"
	done
}
