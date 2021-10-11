#!/usr/bin/env bash

# Installs all dependencies for a typical Lemons workspace, as well as Ccache.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

readonly deps_list="$script_dir/deps_list.txt"

#

# Append text to file
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

####################################################################

# OS specific configuration functions

run_linux_config() {

	sudo apt-get update

	# install deps listed in deps_list file
	xargs sudo apt-get install -y --no-install-recommends <"$deps_list"

	# ccache
	sudo /usr/sbin/update-ccache-symlinks
	append_to_file "$SHELL_FILE" 'export PATH="/usr/lib/ccache:$PATH"'

	# run juce linux deps install script
	chmod u+x "$script_dir/install_juce_linux_deps.sh"
	bash "$script_dir/install_juce_linux_deps.sh"
}

###

run_mac_config() {

	apple_m1() {
		sysctl -n machdep.cpu.brand_string | grep "Apple M1"
	}

	rosetta() {
		uname -m | grep "x86_64"
	}

	homebrew_installed_m1() {
		apple_m1 && ! rosetta && [ -d "/opt/homebrew" ]
	}

	homebrew_installed_intel() {
		! apple_m1 && command -v brew >/dev/null
	}

	local -r brewfile="$script_dir/Brewfile"

	if [ ! -f "$brewfile" ]; then 
		touch "$brewfile"
	fi

	# Install Homebrew, if needed
	if ! [[ homebrew_installed_m1 || homebrew_installed_intel ]]; then
		echo "Installing Homebrew..."
		/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
		brew tap Homebrew/bundle
	fi

	# write deps list to Brewfile
	cat "$deps_list" | while read line || [[ -n $line ]];
	do
		local newtext="$(echo "brew "\'"$line"\')"
		append_to_file "$brewfile" "$newtext"
	done

	# run Homebrew on generated Brewfile
	brew update
	brew bundle install --file="$brewfile"

	# prepend ccache to path
	append_to_file "$SHELL_FILE" 'export PATH="$(brew --prefix)/opt/ccache/libexec:$PATH"'
}

###

run_windows_config() {
	printf "Windows!"
}

####################################################################

#
# Run appropriate OS config function

case "$OSTYPE" in
	darwin*) :
    	run_mac_config
    	;;
  	linux-gnu*) :
    	run_linux_config
    	;;
  	cygwin*) :
		run_windows_config
		;;
	msys*) :	
		run_windows_config
		;;
	win32) :
		run_windows_config
		;;
	*) :
		echo "Error - unknown operating system!"
		exit 1
	;;
esac

source "$SHELL_FILE"

exit 0
