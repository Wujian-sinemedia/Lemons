#!/usr/bin/env bash

# Installs a list of dependencies using apt

set -euo pipefail

sudo apt-get update

# install deps listed in deps list file
xargs sudo apt-get install -y --no-install-recommends <"$DEPS_LIST"

# ccache
sudo /usr/sbin/update-ccache-symlinks
append_to_file "$SHELL_FILE" 'export PATH="/usr/lib/ccache:$PATH"'

# run juce linux deps install script
chmod u+x "$SCRIPT_DIR/../install_juce_linux_deps.sh"
bash "$SCRIPT_DIR/../install_juce_linux_deps.sh"

exit 0
