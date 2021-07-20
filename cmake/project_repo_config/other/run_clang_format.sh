#!/bin/bash

CLANG_FORMAT=${CLANG_FORMAT}
PROJECT_SOURCE_DIR=${BV_PROJECT_SOURCE_DIR}


CLANG_FORMAT_COMMAND="$CLANG_FORMAT -i *.*"


# Run clang format recursively over all subdirectories of the project repo

cd $PROJECT_SOURCE_DIR && $CLANG_FORMAT_COMMAND

DIRS=$(find $PROJECT_SOURCE_DIR -type d)

for dir in $DIRS; do
	cd $dir && $CLANG_FORMAT_COMMAND
done

exit 0;