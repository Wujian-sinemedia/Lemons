#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os


def main():

	LEMONS_ROOT = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

	CODEOWNERS = os.path.join(LEMONS_ROOT, 'CODEOWNERS')

	with open(CODEOWNERS, 'r') as f:
		file_lines = f.readlines()

	# remove empty lines
	file_lines = list(filter(None, file_lines))

	# sort remaining lines
	file_lines.sort()

	with open(CODEOWNERS, 'w') as f:
		f.truncate(0)
		f.write(''.join(file_lines))


if __name__ == '__main__':
	main()
