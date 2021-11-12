#!/usr/bin/env python

import os
from argparse import ArgumentParser

import source_scanner
from generate_translations import generate_translation_files
import options

import concurrent.futures

#

def make_template_file_name (working_dir):
	return working_dir[working_dir.rfind ("/")+1:] + options.translation_file_xtn


def generate_template_file_if_needed (working_dir, cache_dir):

	if not os.path.isdir (working_dir):
		raise Exception("Non existant working directory for template file generation!")
		return ""

	translations_file_abs_path = os.path.join (cache_dir, make_template_file_name (working_dir))

	if not os.path.exists (translations_file_abs_path):

		needed_translations = source_scanner.get_translate_tokens_for_source_tree (os.path.join (working_dir, "modules"))

		with open (translations_file_abs_path, "w") as f:
			for translation in needed_translations:
				f.write ("\"{t}\" = \"\"\r\n".format (t=translation))

	return translations_file_abs_path

#

def process_file_for_lines (file_path):

	lines_to_merge = []

	with open (file_path, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if not stripped_line in lines_to_merge:
						lines_to_merge.append (stripped_line)

	return lines_to_merge


def merge_translation_files (output_file, files_to_merge):

	merged_file = []

	for file in files_to_merge:
		merged_file += process_file_for_lines (file)

	merged_file = source_scanner.remove_duplicates (merged_file)
	merged_file.sort()

	with open (output_file, "w") as f:
		f.write ("\r\n".join (merged_file))

#

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("juce_root", help="the absolute path to the JUCE repo root")
	parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")
	parser.add_argument ("output_dir", help="the absolute path of the directory to write the shared translated files to")
	parser.add_argument ("language_list", help="absolute path to a list of target languages")
	
	args = parser.parse_args()

	if not os.path.isdir (args.output_dir):
		os.mkdir (args.output_dir)

	templates = []

	# generate template files for Lemons and JUCE, if needed
	with concurrent.futures.ThreadPoolExecutor(max_workers=2) as executor:

		futures = {executor.submit (generate_template_file_if_needed, repo_dir, args.output_dir): repo_dir for repo_dir in [args.lemons_root, args.juce_root]}
		
		for future in concurrent.futures.as_completed (futures):
			templates.append (future.result())

	aggregate_file = os.path.join (args.output_dir, "translations.txt")

	merge_translation_files (aggregate_file, templates)

	generate_translation_files (aggregate_file, args.output_dir, args.language_list)
