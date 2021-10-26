#!/usr/bin/env python

from os import path
from os import mkdir
from translate import Translator

import translation_file_parser as parser
from country_codes import get_country_codes_for_langauge
import options

###############################################################################

# Generates a translation file for a certain language based on a template translation file

def generate_translation_file (template_file, output_directory, output_language):

	if not path.exists (template_file):
		raise Exception("Non existant template file!")
		return

	print ("Translating into " + output_language + "...")

	output_file = path.join (output_directory, 
							 options.translation_file_prefix + output_language + options.translation_file_xtn)

	prev_tokens = parser.get_tokens_in_file (output_file)

	translator = Translator(from_lang=options.source_language, to_lang=output_language, email=options.email)

	with open (template_file, "r") as template:
		with open (output_file, "a") as output:
			for line in template:
				token = parser.get_token_for_line (line)

				if not token:
					continue

				if token in prev_tokens:
					continue

				output.write ("\r\n")
				output.write ("\"" + token + "\" = \"" + translator.translate (token) + "\"")

	with open (output_file, "r") as f:
		content = f.read()

	if not content.startswith ("language:"):
		with open (output_file, "w") as f:
			f.write ("language: {l}".format(l=output_language))
			f.write ("\r\n")
			f.write ("countries: {c}".format(c=get_country_codes_for_langauge (output_language)))
			f.write ("\r\n")
			f.write (content)

###############################################################################

def generate_translation_files (template_file, output_dir):

	if not path.isdir (output_dir):
		mkdir (output_dir)

	script_dir = path.abspath (path.dirname (__file__))
	language_list = path.join (script_dir, "translation_config.txt")

	if not path.exists (language_list):
		raise Exception("Nonexistant language list file!")

	with open (language_list, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				generate_translation_file (template_file, output_dir, stripped_line)

