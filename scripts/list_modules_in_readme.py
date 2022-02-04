#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

import os


def main():
	LEMONS_ROOT = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

	README = os.path.join(LEMONS_ROOT, 'README.md')

	MODULES_DIR = os.path.join(LEMONS_ROOT, 'modules')

	module_readme_lines = []

	for dirname in os.listdir(MODULES_DIR):
		dirpath = os.path.join(MODULES_DIR, dirname)

		if not os.path.isdir(dirpath):
			continue

		module_readme_lines.append(f'- {dirname}\n')

		for module_dirname in os.listdir(dirpath):
			module_dirpath = os.path.join(dirpath, module_dirname)

			if not os.path.isdir(module_dirpath):
				continue

			module_readme_lines.append(f'\t- {module_dirname}\n')

	with open(README, 'r') as f:
		readme_lines = f.readlines()

	module_section_start = readme_lines.index(
	    'Here\'s a list of the juce modules in this repository:\n')

	module_section_end = readme_lines.index('---\n', module_section_start)

	new_readme_lines = readme_lines[:module_section_start + 1]

	new_readme_lines.extend(module_readme_lines)

	new_readme_lines.append('\n')

	new_readme_lines.extend(readme_lines[module_section_end:])

	if new_readme_lines != readme_lines:
		with open(README, 'w') as f:
			f.write(''.join(new_readme_lines))


if __name__ == '__main__':
	main()
