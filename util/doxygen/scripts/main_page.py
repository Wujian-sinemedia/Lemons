#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from argparse import ArgumentParser

#

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument("lemons_root",
	                    help="the absolute path to the Lemons repo root")
	parser.add_argument(
	    "main_page_text",
	    help="absolute path to the text to preprend to the main page")
	parser.add_argument("output_file", help="absolute path to write to")

	args = parser.parse_args()

	lemons_readme = os.path.join(args.lemons_root, "README.md")

	last_bit_of_intro = "whatever floats your git boat."

	with open(lemons_readme, "r") as f:
		content = f.read()

	idx = content.find(last_bit_of_intro)

	if idx >= 0:
		content = content[idx + len(last_bit_of_intro):]

	with open(args.main_page_text, "r") as f:
		content = f.read() + content

	output_dir = os.path.abspath(os.path.dirname(args.output_file))

	if not os.path.isdir(output_dir):
		os.makedirs(output_dir, exist_ok=True)

	with open(args.output_file, "w") as f:
		f.write(content)
