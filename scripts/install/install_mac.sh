#!/usr/bin/env bash

# Installs a list of dependencies using Homebrew

set -euo pipefail

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

delete_file_if_exists() {
	if [ -f "$1" ]; then
		rm -f "$1"
	fi
}

#

readonly brewfile="$SCRIPT_DIR/Brewfile"

# Install Homebrew, if needed
if ! [[ homebrew_installed_m1 || homebrew_installed_intel ]]; then
	echo "Installing Homebrew..."
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
	brew tap Homebrew/bundle
fi

delete_file_if_exists "$brewfile"
delete_file_if_exists "$SCRIPT_DIR/Brewfile.lock.json"

# create empty Brewfile, to write to
touch "$brewfile"

# write deps list to Brewfile
cat "$DEPS_LIST" | while read line || [[ -n $line ]];
do
	newtext="$(echo "brew "\'"$line"\')"
	append_to_file "$brewfile" "$newtext"
done

# run Homebrew on generated Brewfile
brew update
brew upgrade
brew bundle install --file="$brewfile"

exit 0
