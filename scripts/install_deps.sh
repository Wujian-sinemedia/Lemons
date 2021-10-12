#!/usr/bin/env bash

# Installs all dependencies for a typical Lemons workspace, as well as ccache.
# If you're on Linux, this script will also run the 'install_juce_linux_deps.sh' script in this directory.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

export INSTALL_LIST_FILE="deps_list.txt"

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "$script_path/install/install_list.sh"



case "$OSTYPE" in 
	linux-gnu*) :
		# prepend ccache to path
		sudo /usr/sbin/update-ccache-symlinks
		append_to_file "$SHELL_FILE" 'export PATH="/usr/lib/ccache:$PATH"'

		# install juce linux deps
		export INSTALL_LIST_FILE="juce_linux_deps.txt"
		bash "$script_path/install/install_list.sh"
	;;
	darwin*) :
		# prepend ccache to path
		append_to_file "$SHELL_FILE" 'export PATH="$(brew --prefix)/opt/ccache/libexec:$PATH"'
	;;
	*) :
	break;
	;;
esac

source "$SHELL_FILE"

exit 0
