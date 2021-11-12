#!/usr/bin/env python

from os import path
from os import mkdir
from translate import Translator

from translation_file_parser import get_tokens_in_file
from country_codes import get_country_codes_for_langauge
import options

import concurrent.futures

#

def generate_translation_file (output_language, tokens_to_translate, output_directory):

	output_file = path.join (output_directory, 
							 options.translation_file_prefix + output_language + options.translation_file_xtn)

	for token in get_tokens_in_file (output_file):
		if token in tokens_to_translate:
			tokens_to_translate.remove (token)
			if len(tokens_to_translate) < 1: return

	print ("Translating into " + output_language + "...")

	translator = Translator(from_lang=options.source_language, to_lang=output_language, email=options.email)

	with open (output_file, "a") as output:
		for token in tokens_to_translate:
			output.write ("\r\n")
			output.write ("\"{t}\" = \"{n}\"".format(t=token, n=translator.translate (token)))

	with open (output_file, "r") as f:
		content = f.read()

	if not content.startswith ("language:"):
		with open (output_file, "w") as f:
			f.write ("language: {l}".format(l=output_language))
			f.write ("\r\n")
			f.write ("countries: {c}".format(c=get_country_codes_for_langauge (output_language)))
			f.write ("\r\n")
			f.write (content)

#

def generate_translation_files (template_file, output_dir, language_list):

	if not path.isdir (output_dir):
		mkdir (output_dir)

	if not path.exists (language_list):
		raise Exception("Nonexistant language list file!")
		return

	if not path.exists (template_file):
		raise Exception("Non existant template file!")
		return

	tokens_to_translate = get_tokens_in_file (template_file)

	languages = []

	with open (language_list, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				languages.append (stripped_line)

	if not languages: 
		raise Exception("Warning: empty language list!")
		return

	with concurrent.futures.ThreadPoolExecutor(max_workers=len(languages)) as executor:
		for language in languages:
			executor.submit (generate_translation_file, language, tokens_to_translate, output_dir)

