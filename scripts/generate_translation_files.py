#!/usr/bin/env python

import os
from translate import Translator
from argparse import ArgumentParser


###############################################################################

# Returns a list of ISO country codes for a language name

def get_country_codes_for_langauge (language):
	country_codes = []
	return country_codes


###############################################################################

# Generates a translation file for a certain language based on a template translation file

def generate_translation_file (template_file, output_directory, output_language):

	if not os.path.exists (template_file):
		raise Exception("Non existant template file!")
		return

	output_file = os.path.join (output_directory, output_language + ".txt")

	if os.path.exists (output_file):
		os.remove (output_file)

	translator = Translator(from_lang="English", to_lang=output_language, email="ben.the.vining@gmail.com")

	with open (template_file, "r") as template:
		with open (output_file, "w") as output:

			output.write ("language: " + output_language)

			output.write ("\n")

			output.write ("countries: " + "".join (get_country_codes_for_langauge (output_language)))

			output.write ("\n")

			for line in template:
				idx = line.find ("\" =")

				if idx < 0:
					continue

				token = line[:idx]

				if token.startswith ("\""):
					token = token[1:]

				if not token:
					continue

				output.write ("\n")
				output.write ("\"" + token + "\" = \"" + translator.translate (token) + "\"")


###############################################################################

# Main script

if __name__ == "__main__":

	script_dir = os.path.abspath (os.path.dirname (__file__))
	language_list = os.path.join (script_dir, "translation_config.txt")

	if not os.path.exists (language_list):
		raise Exception("Nonexistant language list file!")

	parser = ArgumentParser()
	parser.add_argument ("template_file", help="the directory to search for source files")
	parser.add_argument ("output_dir", help="the output directory")
	
	args = parser.parse_args()

	with open (language_list, "r") as f:
		for line in f:
			stripped_line = line.strip()

			if stripped_line:
				generate_translation_file (args.template_file, args.output_dir, stripped_line)
