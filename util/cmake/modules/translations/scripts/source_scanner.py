#!/usr/bin/env python

from os import path
from os import walk

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

		end_idx = text.find (")")

		if end_idx < 0:
			return needed_translations

		spliced = text[:end_idx]
		spliced = spliced.replace (token, "")
		spliced = spliced.strip()

		text = text[end_idx:]

		if spliced.startswith ("("):
			spliced = spliced[1:]

		if spliced.endswith("\""):
			spliced = spliced[:-1]

		spliced = spliced.replace ("\"", "'\"'")
		
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

	if not path.exists (file_path):
		return needed_translations

	filename, file_extension = path.splitext (file_path)
	if not (file_extension == ".h" or file_extension == ".hpp" or file_extension == ".c" or file_extension == ".cpp"):
		return needed_translations
	
	try:
		with open (file_path, "r") as f:
			for line in f:
				stripped_line = line.strip()
				if stripped_line:
					needed_translations += scan_line (stripped_line)
	except UnicodeDecodeError:
		return needed_translations

	return needed_translations


###############################################################################

# Scans all the source files in a given directory

def scan_directory (dir_path):

	needed_translations = []

	if not path.isdir (dir_path):
		return needed_translations

	for dirpath, dirnames, filenames in walk (dir_path):
		for dirname in dirnames:
			needed_translations += scan_directory (path.join (dirpath, dirname))
		for filename in filenames:
			needed_translations += scan_file (path.join (dirpath, filename))

	return needed_translations


###############################################################################

# Remove duplicates from an array

def remove_duplicates (orig_list):

	return list(set(orig_list))
	

###############################################################################

# Returns a sorted list of tokens to be translated for a given source tree

def get_translate_tokens_for_source_tree (dir_to_scan):

	if not path.isdir (dir_to_scan):
		raise Exception("Nonexistant source tree!")
		return

	needed_translations = remove_duplicates (scan_directory (dir_to_scan))
	needed_translations.sort()

	return needed_translations
