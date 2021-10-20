#!/usr/bin/env bash

set -euo pipefail

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "$script_dir/.."

readonly tag_name="juce_dev_machine:latest"

docker build --tag "$tag_name" .

docker push benvining/"$tag_name"

exit 0
