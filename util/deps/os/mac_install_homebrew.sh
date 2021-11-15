#!/bin/bash

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

if ! [ homebrew_installed_m1 || homebrew_installed_intel ]; then
	echo "Installing Homebrew..."
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

exit 0
