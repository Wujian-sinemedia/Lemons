#!/bin/sh

# Installs a list of dependencies using apt

set -euo pipefail

sudo apt-get update
sudo apt-get upgrade

os_install_func() {
	xargs sudo apt-get install -y --no-install-recommends <"$1"
}
