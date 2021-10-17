#!/usr/bin/env bash

# Installs a list of dependencies, read from a given text file

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

#

# Utility function to append text to file
append_to_file() {
  	local -r file="$1"
  	local -r text="$2"

  	if ! grep -qs "^$text$" "$file"; then
    	printf "\\n%s\\n" "$text" >> "$file"
  	fi
}

#

# Detect, and create, if necessary, the shell file

case "$SHELL" in
	*/zsh) :
		export SHELL_FILE="$HOME/.zshrc"
    	;;
  	*/bash) :
  		export SHELL_FILE="$HOME/.bash_profile"
    	;;
  	*/fish) :
		if [ ! -d "$HOME/.config/fish" ]; then
 			mkdir "$HOME/.config/fish"
 		fi

  		export SHELL_FILE="$HOME/.config/fish/config.fish"
    	;;
    *) :
		echo "Error - unknown shell!"
		exit 1
	;;
esac

if [ ! -f "$SHELL_FILE" ]; then
	touch "$SHELL_FILE"
fi

#

# import the appropriate os-specific function

case "$OSTYPE" in
	darwin*) :
		readonly os_script="install_mac"
    	;;
  	linux-gnu*) :
		readonly os_script="install_linux"
    	;;
  	cygwin*) :
		readonly os_script="install_windows"
		;;
	msys*) :	
		readonly os_script="install_windows"
		;;
	win32) :
		readonly os_script="install_windows"
		;;
	*) :
		echo "Error - unknown operating system!"
		exit 1
		;;
esac

source "$SCRIPT_DIR/os/$os_script.sh"

#

# The argument given to this function must be the absolute path to a text file from which to read the list of dependencies
install_deps_list() {
	
	local -r DEPS_LIST="$1"

	# if deps list file doesn't exist, fail with error
	if [ ! -f "$DEPS_LIST" ]; then
		echo "Error - dependency list file not found!"
		echo "Requested file: $DEPS_LIST"
		exit 1
	fi

	echo "Installing dependencies from file: $DEPS_LIST" 

	# run os install func
	os_install_func "$DEPS_LIST"
}
