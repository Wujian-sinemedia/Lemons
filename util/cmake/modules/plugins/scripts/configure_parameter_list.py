#!/usr/bin/env python

import os
from json import loads as load_json
from argparse import ArgumentParser

#

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("input_file", help="absolute path to the JSON file to read")

	args = parser.parse_args()

