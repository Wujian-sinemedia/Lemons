#!/bin/bash


function scan_subdirectory () 
{
	cd "$1"
	printf "Scanning $1..."

	NUM_SUBDIRS=0;

	for f in *; do
	    if [[ -d "$f" && ! -L "$f" ]]
	    then
	        NUM_SUBDIRS=$((NUM_SUBDIRS+1))
		fi
	done

	if [ "$NUM_SUBDIRS" -gt "0" ]
	then 
		for f in *; do
		    if [[ -d "$f" && ! -L "$f" ]]
		    then
		        scan_subdirectory "$f"
		    fi
		done
	else
		clang-format -i *.*
	fi
}

#

scan_subdirectory "$PWD"