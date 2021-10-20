#!/usr/bin/env python

import os
import re
import argparse


###############################################################################

def process_log_file (input_file, output_file):

	with open (input_file, "r") as f:
		orig_log = f.read()


###############################################################################


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument ("working_dir", help="the working directory to run in")
	parser.add_argument ("log_to_process", help="the input log to process")
	parser.add_argument ("output_file", help="the output log file")
	
	args = parser.parse_args()

	process_log_file (os.path.join (args.working_dir, args.log_to_process),
				      os.path.join (args.working_dir, args.output_file))
