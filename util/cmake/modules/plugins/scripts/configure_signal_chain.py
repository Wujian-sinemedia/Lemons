#!/usr/bin/env python

import os
from json import loads as load_json
from argparse import ArgumentParser

#

def get_classes_list (array):

	classes = ""

	for class_obj in array:
		if classes: classes += ", "
		classes += class_obj["type"]

	return classes

#

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("output_file", help="the absolute path to the output file to write to")
	parser.add_argument ("input_file", help="absolute path to the JSON file to read")

	args = parser.parse_args()

	if not os.path.exists (args.input_file):
		print ("Error - JSON input file '{f}' does not exist!".format(f=args.input_file))
		exit (1)

	template_file = os.path.join (os.path.abspath (os.path.dirname (__file__)), "DSP_chain_template.h")

	with open (template_file, "r") as f:
		file_contents = f.read()

	with open (args.input_file, "r") as f:
		json_contents = f.read()

	json = load_json (json_contents)

	chainJson = json["DSP_chain"]

	file_contents = file_contents.replace ("@NAMESPACE@", chainJson["namespace"])
	file_contents = file_contents.replace ("@CLASS_NAME@", chainJson["class_name"])
	file_contents = file_contents.replace ("@ENGINE_TYPES@", get_classes_list (chainJson["nodes"]))

	output_dir = os.path.abspath (os.path.dirname (args.output_file))

	if not os.path.isdir (output_dir):
		os.makedirs (output_dir)

	with open (args.output_file, "w") as f:
		f.write (file_contents)
