#!/usr/bin/env python

import os
from argparse import ArgumentParser


###############################################################################

# Returns an array of strings to look for, the most common ways that translate() appears in source code

def get_tokens_for_symbol (symbol):
	tokens = []

	for ending in ["(\"", " (\"", "( \"", " ( \""]:
		tokens.append (symbol + ending)

	return tokens


def get_translate_tokens():
	tokens = []

	for version in ["translate", "juce::translate", "TRANS"]:
		tokens += get_tokens_for_symbol (version)

	return tokens


###############################################################################

# Scans a single line of source code for calls to translation functions

def scan_line_for_token (text, token):
	needed_translations = []

	while text:
		begin_idx = text.find (token)

		if begin_idx < 0:
			return needed_translations

		text = text[begin_idx:]

		# Find the end of the translation call
		for ending_token in ["\")", "\" )", ")"]:
			end_idx = text.find (ending_token)

			if end_idx < 0:
				continue

			spliced = text[:end_idx]
			spliced = spliced.replace (token, "")
			spliced = spliced.strip()
			
			text = text[end_idx:]

			if spliced and spliced != "\"":
				needed_translations.append (spliced)

	return needed_translations


def scan_line (text):
	needed_translations = []

	for token in get_translate_tokens():
		needed_translations += scan_line_for_token (text, token)

	return needed_translations


###############################################################################

# Scans a single source file for calls to TRANS()

def scan_file (file_path):

	needed_translations = []

	if not os.path.exists (file_path):
		return needed_translations

	filename, file_extension = os.path.splitext (file_path)
	if not (file_extension == ".h" or file_extension == ".hpp" or file_extension == ".c" or file_extension == ".cpp"):
		return needed_translations
	
	with open (file_path, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				needed_translations += scan_line (stripped_line)

	return needed_translations


###############################################################################

# Scans all the source files in a given directory

def scan_directory (dir_path):

	needed_translations = []

	if not os.path.isdir (dir_path):
		return needed_translations

	for dirpath, dirnames, filenames in os.walk (dir_path):
		for dirname in dirnames:
			needed_translations += scan_directory (os.path.join (dirpath, dirname))
		for filename in filenames:
			needed_translations += scan_file (os.path.join (dirpath, filename))

	return needed_translations


###############################################################################

# Remove duplicates from an array

def remove_duplicates (orig_list):
	new_list = []

	for item in orig_list:
		if item not in new_list:
			new_list.append (item)

	return new_list


###############################################################################

# Main script 

if __name__ == "__main__":

	script_dir = os.path.abspath (os.path.dirname (__file__))
	lemons_root = os.path.abspath (os.path.dirname (script_dir))
	lemons_modules = os.path.join (lemons_root, "modules")

	parser = ArgumentParser()
	parser.add_argument ("source_dir", help="the directory to search for source files")
	parser.add_argument ("output_file", help="the file to write the output to")
	
	args = parser.parse_args()

	if os.path.exists (args.output_file):
		os.remove (args.output_file)

	if not os.path.isdir (args.source_dir):
		raise Exception("Non existent source directory!")

	needed_translations = scan_directory (args.source_dir)
	
	if args.source_dir != lemons_modules and args.source_dir != lemons_root:
		needed_translations += scan_directory (lemons_modules)

	needed_translations = remove_duplicates (needed_translations)
	needed_translations.sort()

	with open (args.output_file, "w") as f:
		f.write ("language: \n")
		f.write ("countries: \n\n")

		for translation in needed_translations:
			f.write ("\"{t}\" = \"\"\n".format (t=translation))
