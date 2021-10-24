#!/usr/bin/env python

import os
from argparse import ArgumentParser


###############################################################################

# Main script

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("template_file", help="the directory to search for source files")
	
	args = parser.parse_args()

	if not os.path.exists (args.template_file):
		raise Exception("Non existant template file!")

	