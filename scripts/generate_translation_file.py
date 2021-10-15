#!/usr/bin/env python

import os
import re


###############################################################################

# Returns an array of strings to look for, the most common ways that translate() appears in source code

def get_translate_tokens():
	tokens = []

	tokens.append ("translate(\"")
	tokens.append ("translate (\"")
	tokens.append ("translate( \"")
	tokens.append ("translate ( \"")
	tokens.append ("TRANS(\"")
	tokens.append ("TRANS (\"")
	tokens.append ("TRANS( \"")
	tokens.append ("TRANS ( \"")

	return tokens


###############################################################################

# Scans a single source file for calls to TRANS()

def scan_file (file_path):
	
	with open (file_path, "r") as f:
		content = f.read()

	needed_translations = []

	for token in get_translate_tokens():
		for result in re.split (r'%token', content):
			print (result)
			needed_translations.append (result)

	return "\r\n".join (needed_translations)


###############################################################################

# Scans all the source files in a given directory

def scan_directory (dir_path):

	needed_translations = []

	for dirpath, dirnames, filenames in os.walk (dir_path):
		for dirname in dirnames:
			needed_translations.append (scan_directory (os.path.join (dirpath, dirname)))
		for filename in filenames:
			needed_translations.append (scan_file (os.path.join (dirpath, filename)))

	return "\r\n".join (needed_translations)


###############################################################################

# Remove duplicates from a list

def remove_duplicates (orig_list):
	return list (set (orig_list))


###############################################################################

# Main script 

if __name__ == "__main__":

	script_dir = os.path.abspath (os.path.dirname (__file__))

	repo_dir = os.path.abspath (os.path.dirname (script_dir))

	modules_dir = os.path.join (repo_dir, "modules")

	output_file = os.path.join (script_dir, "lemons_translations.txt")

	if os.path.exists (output_file):
		os.remove (output_file)

	needed_translations = remove_duplicates (scan_directory (modules_dir))

	# write to output file
	with open (output_file, "w") as f:
		f.write ("\r\n".join (needed_translations))
