#!/usr/bin/env python

import os
from translate import Translator
from argparse import ArgumentParser


###############################################################################

# Generates a translation file for a certain language based on a template translation file

def generate_translation_file (template_file, output_directory, template_language, output_language):

	if not os.path.exists (template_file):
		raise Exception("Non existant template file!")
		return

	output_file = os.path.join (output_directory, output_language + ".txt")

	if os.path.exists (output_file):
		os.remove (output_file)

	translator = Translator(from_lang=template_language, to_lang=output_language)

	with open (template_file, "r") as template:
		with open (output_file, "w") as output:

			# TODO: write first 2 lines of file...

			for line in template:
				idx = line.find ("\" =")

				if idx < 0:
					continue

				token = line[:idx]

				if token.startswith ("\""):
					token = token[1:]

				finished_line = "\"" + token + "\" = \"" + translator.translate (token) + "\""

				output.write (finished_line)


###############################################################################

# Main script

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("template_file", help="the directory to search for source files")
	parser.add_argument ("output_dir", help="the output directory")
	
	args = parser.parse_args()

	generate_translation_file (args.template_file, args.output_dir, "English", "German")
