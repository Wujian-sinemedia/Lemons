#!/usr/bin/env bash

set -euo pipefail

os_install_func() {
	local -r DEPS_LIST="$1"

	# install deps listed in deps list file
	xargs choco install <"$DEPS_LIST"
}
