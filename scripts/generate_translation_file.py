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

	tokens.append ("juce::translate(\"")
	tokens.append ("juce::translate (\"")
	tokens.append ("juce::translate( \"")
	tokens.append ("juce::translate ( \"")

	tokens.append ("TRANS(\"")
	tokens.append ("TRANS (\"")
	tokens.append ("TRANS( \"")
	tokens.append ("TRANS ( \"")

	return tokens


###############################################################################

# Scans a single line of source code for calls to TRANS()

def scan_line (text):
	needed_translations = []
	return needed_translations


###############################################################################

# Scans a single source file for calls to TRANS()

def scan_file (file_path):

	needed_translations = []

	filename, file_extension = os.path.splitext (file_path)
	if not (file_extension == ".h" or file_extension == ".hpp" or file_extension == ".c" or file_extension == ".cpp"):
		return needed_translations
	
	with open (file_path, "r") as file:
		for line in file:
			if line:
				for translation in scan_line (line):
					needed_translations.append (translation)

	return needed_translations


###############################################################################

# Scans all the source files in a given directory

def scan_directory (dir_path):

	needed_translations = []

	for dirpath, dirnames, filenames in os.walk (dir_path):
		for dirname in dirnames:
			for translation in scan_directory (os.path.join (dirpath, dirname)):
				needed_translations.append (translation)
		for filename in filenames:
			for translation in scan_file (os.path.join (dirpath, filename)):
				needed_translations.append (translation)

	return needed_translations


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
		f.write ("language: ")
		f.write ("countries: ")
		f.write ("")

		for translation in needed_translations:
			f.write ("\"{t}\" = ".format (t=translation))
