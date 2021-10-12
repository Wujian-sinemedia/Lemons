#!/usr/bin/env bash

# Installs a list of dependencies using apt

set -euo pipefail

sudo apt-get update

# install deps listed in deps list file
xargs sudo apt-get install -y --no-install-recommends <"$DEPS_LIST"

exit 0
