#!/usr/bin/env bash

# Installs all tools with which Lemons can integrate.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

export INSTALL_LIST_FILE="integrations_list.txt"

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

bash "$script_path/install/install_list.sh"

exit 0