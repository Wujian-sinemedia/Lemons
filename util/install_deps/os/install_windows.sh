#!/bin/sh

# Installs a list of dependencies using Chocolatey

set -euo pipefail

# TO DO: Check if Chocolatey is installed, and install it if need be...?


process_line() {
	line=$(echo "$1" | sed 's/;//')

	if [ "$line" = "doxygen" ]; then
		return
	fi

	choco install "$line"
}


os_install_func() {

	cat "$1" | while read line || [ -n "$line" ];
	do
		process_line "$line"
	done
}
