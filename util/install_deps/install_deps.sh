#!/usr/bin/env bash

# Installs all dependencies for a typical Lemons workspace
# If you're on Linux, this script will also install all of JUCE's Linux dependencies.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "$script_path/install_list.sh"

install_deps_list "$script_path/deps_list.txt"


case "$OSTYPE" in 
	linux-gnu*) :
		# prepend ccache to path
		sudo /usr/sbin/update-ccache-symlinks
		append_to_file "$SHELL_FILE" 'export PATH="/usr/lib/ccache:$PATH"'

		# install juce linux deps
		install_deps_list "$script_path/juce_linux_deps.txt"
	;;
	darwin*) :
		# prepend ccache to path
		append_to_file "$SHELL_FILE" 'export PATH="$(brew --prefix)/opt/ccache/libexec:$PATH"'
	;;
	*) :
	;;
esac


CROSSCOMPILE_ANDROID="${CROSSCOMPILE_ANDROID:-false}"
CROSSCOMPILE_ANDROID=$( echo "$CROSSCOMPILE_ANDROID" | tr '[:upper:]' '[:lower:]' )

if [ "$CROSSCOMPILE_ANDROID" = true ] ; then
	/usr/bin/env bash "$script_path/os/install_android_deps.sh"
fi


source "$SHELL_FILE"

exit 0
