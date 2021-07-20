#!/bin/bash

CLANG_FORMAT=${CLANG_FORMAT}
PROJECT_SOURCE_DIR=${BV_PROJECT_SOURCE_DIR}

RUN_IN_SOURCE_DIR=${BV_RUN_CLANG_FORMAT_IN_SOURCE_DIR}

CLANG_FORMAT_COMMAND="$CLANG_FORMAT -i *.*"


# Run clang format recursively over all subdirectories of the project repo

cd $PROJECT_SOURCE_DIR

if [[ $RUN_IN_SOURCE_DIR == 1 ]]; then
	echo "Running in source dir..."
	$CLANG_FORMAT_COMMAND
fi

DIRS=$(find $PROJECT_SOURCE_DIR -type d)

for dir in $DIRS; do
	cd $dir && $CLANG_FORMAT_COMMAND
done

exit 0;