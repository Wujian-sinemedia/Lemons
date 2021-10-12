#!/usr/bin/env bash

# Installs all tools with which Lemons can integrate.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "$script_path/install/install_list.sh"

install_deps_list "integrations_list.txt"

exit 0