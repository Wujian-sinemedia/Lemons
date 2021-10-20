#!/usr/bin/env bash

set -euo pipefail

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "$script_dir/.."

readonly tag_name="benvining/juce_dev_machine:latest"

docker buildx create --use --name=qemu

docker buildx inspect --bootstrap

docker buildx build \
  --push -t "$tag_name" \
  --platform="$DOCKER_PLATFORMS" .

exit 0
