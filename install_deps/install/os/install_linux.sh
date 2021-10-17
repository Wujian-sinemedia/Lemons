#!/usr/bin/env bash

# Installs a list of dependencies using apt

sudo apt-get update

os_install_func() {
	xargs sudo apt-get install -y --no-install-recommends <"$1"
}