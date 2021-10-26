#!/usr/bin/env python

import os
from translate import Translator
from argparse import ArgumentParser

import country_codes

###############################################################################

TRANSLATION_FILE_PREFIX = "trans_"
TRANSLATION_FILE_XTN = ".txt"

###############################################################################

# Returns the translated key from a line of a translation file

def get_token_for_line (line):

	idx = line.find ("\" =")

	if idx < 0:
		return ""

	token = line[:idx]

	if token.startswith ("\""):
		token = token[1:]

	return token

###############################################################################

# Generates a translation file for a certain language based on a template translation file

def generate_translation_file (template_file, output_directory, output_language):

	if not os.path.exists (template_file):
		raise Exception("Non existant template file!")
		return

	print ("Translating into " + output_language + "...")

	output_file = os.path.join (output_directory, 
							    TRANSLATION_FILE_PREFIX + output_language + TRANSLATION_FILE_XTN)

	prev_tokens = []

	if os.path.exists (output_file):
		with open (output_file, "r") as f:
			for line in f:
				token = get_token_for_line (line)
				if token:
					prev_tokens.append (token)

	translator = Translator(from_lang="English", to_lang=output_language, email="ben.the.vining@gmail.com")

	with open (template_file, "r") as template:
		with open (output_file, "w") as output:

			output.write ("language: " + output_language)

			output.write ("\n")

			output.write ("countries: " + "".join (country_codes.get_country_codes_for_langauge (output_language)))

			output.write ("\n")

			for line in template:
				token = get_token_for_line (line)

				if not token:
					continue

				if token in prev_tokens:
					output.write (line)
					continue

				output.write ("\n")
				output.write ("\"" + token + "\" = \"" + translator.translate (token) + "\"")


###############################################################################

# Main script

def generate_translations (template_file, output_dir):

	if not os.path.isdir (output_dir):
		os.mkdir (output_dir)

	script_dir = os.path.abspath (os.path.dirname (__file__))
	language_list = os.path.join (script_dir, "translation_config.txt")

	if not os.path.exists (language_list):
		raise Exception("Nonexistant language list file!")

	with open (language_list, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				generate_translation_file (template_file, output_dir, stripped_line)

