#!/usr/bin/env bash

set -euo pipefail

os_install_func() {
	local -r DEPS_LIST="$1"

	# install deps listed in deps list file
	cat "$DEPS_LIST" | while read line || [[ -n $line ]];
	do
		choco install "$line"
	done
}
