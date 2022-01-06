#!/usr/bin/env python

from argparse import ArgumentParser

import os
import sys
import subprocess


def get_cmake_generator_for_system():
	if sys.platform.startswith ("linux"):
		return "Ninja"
	elif sys.platform == "darwin":
		return "Xcode"
	elif sys.platform == "win32" or sys.platform == "cygwin":
		return "\"Visual Studio 16 2019\""
	else:
		raise Exception("Unknown OS!")


def execute_command (command):
	cp = subprocess.run (command, stderr=subprocess.PIPE, stdout=subprocess.PIPE, text=True, shell=True)

	if cp.returncode != 0:
		print ("Command {cmd} failed with exit code {code}".format (cmd=command, code=cp.returncode))

		if cp.stdout:
			print ("Errors from child process: \r\n {e}".format (e=cp.stdout))

		raise ChildProcessError


if __name__ == "__main__":
	parser = ArgumentParser()

	parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")

	args = parser.parse_args()

	if not os.path.isdir (args.lemons_root):
		print ("Lemons root directory does not exist!")
		exit (1)

	generator = get_cmake_generator_for_system()

	os.chdir (args.lemons_root)

	print ("Running CMake from Lemons root...")

	# to do: add editor presets

	for preset_name in "docs", "tests", "utils":
		print ("Configure preset: {p}".format (p=preset_name))
		execute_command ("cmake --preset {p} -G {g}".format(p=preset_name, g=generator))

	for preset_name in "docs", "tests", "utils":
		print ("Build preset: {p}".format (p=preset_name))
		execute_command ("cmake --build --preset {p}".format(p=preset_name))

	# test running cmake from every subdirectory with a project() command
	# to do: add templates, metadata editor
	for subdir in "util/doxygen", "util/UnitTestRunner", "util/CommandLineUtils", "util/CommandLineUtils/FontSerializer", "util/CommandLineUtils/KeyFileGenerator", "util/CommandLineUtils/RSAKeyGenerator", "util/CommandLineUtils/TranslationFileGenerator":
		subdir_path = os.path.join (args.lemons_root, subdir)

		os.chdir (subdir_path)

		print ("Configuring in directory: {d}".format (d=subdir))

		execute_command ("cmake -B Builds -G {g}".format(g=generator))

		print ("Building in directory: {d}".format (d=subdir))

		execute_command ("cmake --build Builds")
