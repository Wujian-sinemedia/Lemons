#!/usr/bin/env bash

# Installs all dependencies for a typical Lemons workspace, as well as ccache.
# If you're on Linux, this script will also run the 'install_juce_linux_deps.sh' script in this directory.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

export INSTALL_LIST_FILE="deps_list.txt"

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "$script_path/install/install_list.sh"

exit 0
