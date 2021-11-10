#!/usr/bin/env python

import os
from json import loads as load_json
from argparse import ArgumentParser

#

def add_automatable_parameters (array):
	for param in array:
		if not param["type"]:
			raise Exception("Undefined parameter type!")
			return
		if not param["name"]:
			raise Exception("Undefined parameter name!")
			return

#

def add_meters (array):
	for param in array:
		if not param["name"]:
			raise Exception("Undefined parameter name!")
			return

#

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("input_file", help="absolute path to the JSON file to read")

	args = parser.parse_args()

	with open (args.input_file, "r") as f:
		json_contents = f.read()

	json = load_json (json_contents)

	add_automatable_parameters (json["automatable"])

	add_meters (json["meters"])
