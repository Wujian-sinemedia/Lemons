#!/usr/bin/env python

from argparse import ArgumentParser

import os
import subprocess
from sys import platform as os_name
from shutil import rmtree


def get_cmake_generator_for_system():
	if os_name.startswith ("linux"):
		return "Ninja"
	elif os_name == "darwin":
		return "Xcode"
	elif os_name == "win32" or "cygwin":
		return "\"Visual Studio 16 2019\""
	else:
		raise Exception("Unknown OS!")


def execute_command (command):

	print ("Executing command: {c}".format (c=command))

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

	# test running cmake from every subdirectory with a project() command
	# to do: add templates, metadata editor
	for subdir in "util/doxygen", "util/UnitTestRunner", "util/CommandLineUtils":
		subdir_path = os.path.join (args.lemons_root, subdir)

		# remove build dir, if it exists
		rmtree (os.path.join (subdir_path, "Builds"), ignore_errors=True)

		os.chdir (subdir_path)

		print ("Configuring in directory: {d}".format (d=subdir))

		execute_command ("cmake -B Builds -G {g}".format(g=generator))

		print ("Building in directory: {d}".format (d=subdir))

		execute_command ("cmake --build Builds")
