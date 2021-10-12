#!/usr/bin/env bash

# Installs a list of dependencies using apt

sudo apt-get update

os_install_func() {
	local -r DEPS_LIST="$1"

	# install deps listed in deps list file
	xargs sudo apt-get install -y --no-install-recommends <"$DEPS_LIST"
}