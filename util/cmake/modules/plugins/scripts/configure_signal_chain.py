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

	script_dir = os.path.abspath (os.path.dirname (__file__))
	template_file = os.path.join (script_dir, "DSP_chain_template.h")

	with open (template_file, "r") as f:
		file_contents = f.read()

	with open (args.input_file, "r") as f:
		json_contents = f.read()

	json = load_json (json_contents)

	file_contents = file_contents.replace ("@LEMONS_DSP_HEADER_NAMESPACE@", json["namespace"])
	file_contents = file_contents.replace ("@LEMONS_DSP_CHAIN_CLASS_NAME@", json["class_name"])
	file_contents = file_contents.replace ("@LEMONS_DSP_CHAIN_CLASSES@", get_classes_list (json["nodes"]))

	output_dir = os.path.abspath (os.path.dirname (args.output_file))

	if not os.path.isdir (output_dir):
		os.makedirs (output_dir)

	with open (args.output_file, "w") as f:
		f.write (file_contents)
