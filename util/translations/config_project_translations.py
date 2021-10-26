#!/usr/bin/env python

import os
from argparse import ArgumentParser
import shutil

import generate_template
import merge_templates
import generate_translations

###############################################################################

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("translated_files_dir", help="the absolute path to the Lemons repo root")
	parser.add_argument ("output_dir", help="the absolute path to the output directory")
	
	args = parser.parse_args()

	if not os.path.isdir (args.translated_files_dir):
		raise Exception("Non existant translated files directory!")

	if os.path.isdir (args.output_dir):
		shutil.rmtree (args.output_dir)
	
	# copy the translated files to the output directory
	shutil.copytree (args.translated_files_dir, args.output_dir)

	# 5 - generate list of needed phrases for the product, and add any not present to each translated file

	# 6 - fill in any non-translated phrases
