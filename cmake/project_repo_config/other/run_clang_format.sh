#!/bin/bash

CLANG_FORMAT=${CLANG_FORMAT}
PROJECT_SOURCE_DIR=${BV_PROJECT_SOURCE_DIR}


# Run clang format recursively over all subdirectories of the project repo

DIRS=$(find $PROJECT_SOURCE_DIR -type d)

for dir in $DIRS; do
	cd $dir && $CLANG_FORMAT -i ${BV_CLANG_FORMAT_FILETYPES}
done

exit 0;