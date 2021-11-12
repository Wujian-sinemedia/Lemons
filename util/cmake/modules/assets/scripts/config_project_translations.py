#!/usr/bin/env python

import os
from translate import Translator
from argparse import ArgumentParser
import shutil

from source_scanner import get_translate_tokens_for_source_tree
from translation_file_parser import get_tokens_in_file
import options

import concurrent.futures

#

def process_translated_file (file_path, needed_translations):

	# detect the language of this translation file
	with open (file_path, "r") as f:
		firstLine = f.readline().rstrip()

	if not firstLine.startswith ("language:"):
		raise Exception("Unknown language for generated translation file!")
		return

	output_language = firstLine[len("language:"):]
	output_language = output_language.strip()

	if not output_language:
		raise Exception("Unknown language for generated translation file!")
		return

	for token in get_tokens_in_file (file_path):
		if token in needed_translations:
			needed_translations.remove (token)
			if len(needed_translations) < 1: return

	translator = Translator(from_lang=options.source_language, to_lang=output_language, email=options.email)

	with open (file_path, "a") as f:
		for token in needed_translations:
			f.write ("\r\n")
			f.write ("\"{o}\" = \"{t}\"".format(o=token, t=translator.translate (token)))

#

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("translated_files_dir", help="the absolute path to the Lemons repo root")
	parser.add_argument ("project_dir", help="the absolute path to the project's source tree to scan for translations")
	parser.add_argument ("output_dir", help="the absolute path to the output directory")
	
	args = parser.parse_args()

	if not os.path.isdir (args.translated_files_dir):
		raise Exception("Non existant translated files directory!")

	if os.path.isdir (args.output_dir):
		shutil.rmtree (args.output_dir)

	os.makedirs (args.output_dir)

	for dirpath, dirnames, filenames in os.walk (args.translated_files_dir):
		for file in filenames:
			if file.startswith (options.translation_file_prefix):
				shutil.copy2 (os.path.join (dirpath, file), os.path.join (args.output_dir, file))
	
	# generate list of needed phrases for the product
	needed_translations = get_translate_tokens_for_source_tree (args.project_dir)

	# make list of absolute paths of pre-generated files containing Lemons & JUCE translations
	filepaths = []

	for dirpath, dirnames, filenames in os.walk (args.output_dir):
		for file in filenames:
			file_path = os.path.join (dirpath, file)
			if os.path.exists (file_path):
				filepaths.append (file_path)

	# scan each translated file, and add any phrases from the product's list that are missing
	with concurrent.futures.ThreadPoolExecutor(max_workers=len(filepaths)) as executor:
		for filepath in filepaths:
			executor.submit (process_translated_file, filepath, needed_translations)
