#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

#


def split_args_from_line(inputString):

    inputString = inputString[inputString.find("(") + 1 : inputString.find(")")]

    res = []
    temp = []

    for word in inputString.split():
        if word in ('""', '"'):
            res.append(" ")
            continue

        if word.startswith('"') and not word.endswith('"'):
            temp.append(word)
            continue

        if temp and not word.endswith('"'):
            temp.append(word)
            continue

        if word.endswith('"'):
            temp.append(word)

            newWord = " ".join(temp)
            if newWord.startswith('"'):
                newWord = newWord[1:]
            if newWord.endswith('"'):
                newWord = newWord[:-1]
            res.append(newWord)

            temp = []
        else:
            res.append(word)

    return res


#


def parse_option_from_line(line):
    args = split_args_from_line(line)

    string = "**{n}:** {t}.".format(n=args[0], t=args[1])

    default = args[2]
    if default and not default == " ":
        string += " Defaults to {d}.".format(d=default)

    string += "\r\n"

    return string


def parse_dependant_option_from_line(line):
    args = split_args_from_line(line)

    return "**{n}:** {t}. \r\n".format(n=args[0], t=args[1])


def get_options_from_cmakelists(orig_text):
    options = []

    for line in orig_text.splitlines():
        stripped_line = line.strip()
        if not stripped_line:
            continue

        if stripped_line.startswith("option"):
            options.append(parse_option_from_line(stripped_line))
        elif stripped_line.startswith("cmake_dependent_option"):
            options.append(parse_dependant_option_from_line(stripped_line))

    return options


#


def parse_cache_var_from_line(line):
    args = split_args_from_line(line)

    if len(args) >= 5:
        string = "**{n}:** {t}.".format(n=args[0], t=args[4])
    else:
        string = "**{n}**".format(n=args[0])

    default = args[1].strip('"')
    if default and not default == " ":
        string += " Defaults to {d}.".format(d=default)

    string += "\r\n"

    return string


def get_cache_vars_from_cmakelists(orig_text):
    cache_vars = []

    for line in orig_text.splitlines():
        stripped_line = line.strip()
        if not stripped_line:
            continue

        if stripped_line.startswith("set"):
            if "CACHE" in line:
                cache_vars.append(parse_cache_var_from_line(stripped_line))

    return cache_vars


#


def process_cmake_api(cmake_api_input, lemons_root):

    lemons_cmakelists = os.path.join(lemons_root, "CMakeLists.txt")

    if not os.path.isfile(lemons_cmakelists):
        raise Exception("Lemons CMakeLists.txt cannot be found!")

    with open(lemons_cmakelists, "r") as f:
        cmakelists_text = f.read()

    output_lines = [
        "# Lemons CMake API        {#CMake_API}",
        "\r\n",
        "## CMake options",
    ]

    options = get_options_from_cmakelists(cmakelists_text)
    options += get_cache_vars_from_cmakelists(cmakelists_text)
    options.sort()

    for option in options:
        if option.startswith("**LEMONS_"):
            output_lines.append(" - {o}".format(o=option))

    output_lines.append("\r\n\r\n --- \r\n\r\n")

    with open(cmake_api_input, "r") as f:
        output_lines += f.readlines()

    return "\r\n".join(output_lines)
