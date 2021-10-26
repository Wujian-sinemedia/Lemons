#!/usr/bin/env python

import os
from argparse import ArgumentParser

import generate_template
import merge_templates
import generate_translations

###############################################################################

def generate_template_file_if_needed (working_dir, name, source_dir_name):

	if not os.path.isdir (working_dir):
		raise Exception("Non existant working directory for template file generation!")
		return ""

	translations_file = name + "_translations.txt"
	translations_file_abs_path = os.path.join (working_dir, translations_file)

	if not os.path.exists (translations_file_abs_path):
		print ("Generating translations template for " + name + "...")
		generate_template.generate_template_file (os.path.join (working_dir, source_dir_name), 
												  translations_file_abs_path)

	return translations_file_abs_path

###############################################################################

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("juce_root", help="the absolute path to the JUCE repo root")
	parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")
	parser.add_argument ("output_dir", help="the absolute path of the directory to write the shared translated files to")
	
	args = parser.parse_args()

	# generate template file for Lemons, if needed
	lemons_template_file = generate_template_file_if_needed (args.lemons_root, "Lemons", "modules")

	# generate template file for JUCE, if needed
	juce_template_file = generate_template_file_if_needed (args.juce_root, "JUCE", "modules")

	# merge both - output is written back to the lemons repo's file
	merge_templates.merge_translation_file (lemons_template_file, juce_template_file)

	# take the master template file and translate it into each target language
	generate_translations.generate_translations (lemons_template_file, args.output_dir)

	os.remove (lemons_template_file)
