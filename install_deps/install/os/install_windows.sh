#!/usr/bin/env bash

# Installs a list of dependencies using Chocolatey

set -euo pipefail

os_install_func() {

	local -r DEPS_LIST="$1"

	xargs choco install <"$DEPS_LIST"
}
