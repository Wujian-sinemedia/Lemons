#!/usr/bin/env python

import os
from argparse import ArgumentParser

import source_scanner
from generate_translations import generate_translation_files


###############################################################################

def generate_template_file_if_needed (working_dir, name, cache_dir):

	if not os.path.isdir (working_dir):
		raise Exception("Non existant working directory for template file generation!")
		return ""

	translations_file = name + "_translations.txt"
	translations_file_abs_path = os.path.join (cache_dir, translations_file)

	if not os.path.exists (translations_file_abs_path):
		print ("Generating translations template for " + name + "...")

		source_dir = os.path.join (working_dir, "modules")

		if os.path.exists (translations_file_abs_path):
			os.remove (translations_file_abs_path)

		needed_translations = source_scanner.get_translate_tokens_for_source_tree (source_dir)

		with open (translations_file_abs_path, "w") as f:
			for translation in needed_translations:
				f.write ("\"{t}\" = \"\"\r\n".format (t=translation))

	return translations_file_abs_path

###############################################################################

def process_file_for_lines (file_path):

	lines_to_merge = []

	with open (file_path, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if not stripped_line in lines_to_merge:
						lines_to_merge.append (stripped_line)

	return lines_to_merge


def merge_translation_file (output_file, *files_to_merge):

	merged_file = []

	for file in files_to_merge:
		merged_file += process_file_for_lines (file)

	merged_file = source_scanner.remove_duplicates (merged_file)

	merged_file.sort()

	with open (output_file, "w") as f:
		f.write ("\r\n".join (merged_file))

###############################################################################

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("juce_root", help="the absolute path to the JUCE repo root")
	parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")
	parser.add_argument ("output_dir", help="the absolute path of the directory to write the shared translated files to")
	
	args = parser.parse_args()

	if not os.path.isdir (args.output_dir):
		os.mkdir (args.output_dir)

	# generate template file for Lemons, if needed
	lemons_template_file = generate_template_file_if_needed (args.lemons_root, "Lemons", args.output_dir)

	# generate template file for JUCE, if needed
	juce_template_file = generate_template_file_if_needed (args.juce_root, "JUCE", args.output_dir)

	aggregate_file = os.path.join (args.output_dir, "translations.txt")

	# merge both into an aggregate template file
	merge_translation_file (aggregate_file, lemons_template_file, juce_template_file)

	# take the master template file and translate it into each target language
	generate_translation_files (aggregate_file, args.output_dir)

	# remove the aggregate file
	os.remove (aggregate_file)
