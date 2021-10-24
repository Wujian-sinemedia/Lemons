#!/usr/bin/env python

from os import path
from argparse import ArgumentParser


###############################################################################

def process_log_file (input_file, output_file, tokenToSearchFor):

	if not path.exists (input_file):
		raise Exception("Nonexistent input log!")
		return

	assert tokenToSearchFor

	output_lines = []

	lower_token = tokenToSearchFor.lower()

	with open (input_file, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if tokenToSearchFor.capitalize() in stripped_line or lower_token in stripped_line:
					output_lines.append (stripped_line)

	num_errors = len(output_lines)

	plural_token = lower_token

	if num_errors != 1:
		plural_token += "s"

	num_errors_string = str(num_errors) + " " + plural_token + " found"

	if num_errors > 0:
		num_errors_string += ":"

	print ("")
	print (num_errors_string)

	if num_errors > 1:
		print ("First " + lower_token + ":")
		print (output_lines[0])

	print ("")

	with open (output_file, "w") as f:
		f.write (num_errors_string)
		f.write ("")
		f.write ("\r\n\r\n".join (output_lines))


###############################################################################


if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("working_dir", help="the working directory to run in")
	parser.add_argument ("log_to_process", help="the input log to process")
	parser.add_argument ("output_file", help="the output log file")
	parser.add_argument ("token", help="the line token to parse in the input log file")
	
	args = parser.parse_args()
	
	process_log_file (path.join (args.working_dir, args.log_to_process),
				      path.join (args.working_dir, args.output_file),
				      args.token.strip())
