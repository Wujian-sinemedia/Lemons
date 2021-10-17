#!/usr/bin/env bash

# Installs a list of dependencies using Chocolatey

set -euo pipefail

os_install_func() {

	local -r DEPS_LIST="$1"

	cat "$DEPS_LIST" | while read line || [[ -n "$line" ]];
	do
		line=$(echo "$line" | sed 's/;//')
		if [ "$line" = "clang-format" ]; then
			continue;
		fi
		echo "line : \"$line\""
		choco install "$line"
	done
}
