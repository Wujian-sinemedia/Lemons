#!/usr/bin/env bash

# Installs a list of dependencies, read from a given text file

# Input 
# INSTALL_LIST_FILE : The name of the file to read deps from, relative to the directory of this script

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export DEPS_LIST="$SCRIPT_DIR/$INSTALL_LIST_FILE"

echo "Installing dependencies from file: $INSTALL_LIST_FILE" 

# if deps list file doesn't exist, fail with error
if [ ! -f "$DEPS_LIST" ]; then
	printf "Error - dependency list file not found!"
	exit 1
fi

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


# Run appropriate OS config script

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

source "$SCRIPT_DIR/$os_script.sh"

source "$SHELL_FILE"

exit 0
