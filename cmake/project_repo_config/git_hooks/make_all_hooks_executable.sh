#!/bin/bash

if [[ $# == 0 ]]
	exit 1;
fi

REPO_DIR=$1

chmod u+x "$REPO_DIR"/.git/hooks/*   

for submodule in "$REPO_DIR"/.git/modules/*; do                                                                                                                           
    chmod u+x "$submodule"/hooks/*                                                                                                                                       
done

exit 0;