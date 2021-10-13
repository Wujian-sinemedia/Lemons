#!/usr/bin/env bash

# Installs all tools with which Lemons can integrate.

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

readonly script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "$script_path/install/install_list.sh"

install_deps_list "integrations_list.txt"

case "$OSTYPE" in 
	linux-gnu*) :
	;;
	darwin*) :
		# install clang-tidy & clang-format
		brew install llvm
		append_to_file "$SHELL_FILE" 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"'
		ln -s "/usr/local/opt/llvm/bin/clang-format" "/usr/local/bin/clang-format"
		ln -s "/usr/local/opt/llvm/bin/clang-tidy" "/usr/local/bin/clang-tidy"
	;;
	*) :
	;;
esac

source "$SHELL_FILE"

exit 0