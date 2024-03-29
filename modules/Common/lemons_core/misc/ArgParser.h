
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons
{

using juce::File;
using juce::juce_wchar;

/** @ingroup lemons_core
	A command-line argument parser class modeled after Python's ArgParse.
	Use one of these to easily set default values and help strings for arguments, and then it can automatically print help and check for required arguments for you. For example:
	@code
	void process_file (const juce::File& file, int someNumber);

	int main (int argc, char** argv)
	{
		lemons::ArgParser args {argc, argv};

		args.addArgument ("--file|-f", true, "Path to the file to read"); // this argument is required and has no default
		args.addArgument ("--number|-n", false, "Some number argument", "6"); // this one isn't required and defaults to "6"

		if (args.checkForHelpFlag())
			return EXIT_SUCCESS;

		if (! args.checkForRequiredArgs())
			return EXIT_FAILURE;

		process_file (args.getFilepathForOption ("--file|-f"), args.getArgumentAsType<int>("--number|-n"));
	}
	@endcode
 */
class ArgParser final
{
public:

	/** Creates an ArgParser from a standard argc/argv pair. */
	explicit ArgParser (int argc, char** argv);

	/** Adds an argument to the parser.
		@param argOrFlags The argument or flags that must be present on the command line. You can specify multiple flags separated by pipes, for example, '--file|-f'.
		@param required Whether or not the parser will require this argument to be present.
		@param help An optional help string for this argument.
		@param defaultValue An optional default value for this argument.
		@param options If specified, the passed argument must be one of these choices.
	*/
	void addArgument (const String&			   argOrFlags,
					  bool					   required		= false,
					  const String&			   help			= {},
					  const String&			   defaultValue = {},
					  const juce::StringArray& options		= {});

	/** Prints the parser's help to std::cout. */
	void printHelp() const;

	/** Returns one of the arguments from the list (or its corresponding default, if there is one).
		@param argOrFlags The argument flags as they were specified when you called addArgument().
	*/
	[[nodiscard]] String operator[] (const String& argOrFlags) const;

	/** Resolves an argument to a filepath.
		If the argument isn't present, this will return a null File object.
		If a relative path was specified on the command line, this will return a file relative to the current working directory.
	*/
	[[nodiscard]] File getFilepathForOption (const String& argOrFlags) const;

	/** Interprets an argument from the list as a specified type.
		Currently supports String, juce_wchar, File, int, int64, float, or double.
		The juce_wchar version returns the first character of the argument string.
	*/
	template <typename Type>
	[[nodiscard]] Type getArgumentAsType (const String& argOrFlags) const;

	/** Returns true if all arguments marked as required were specified on the command line.
		This also verifies that, for any arguments with a list of valid options specified, the argument passed on the command line is one of the valid options.
	*/
	[[nodiscard]] bool checkForRequiredArgs() const;

	/** If '--help' or '-h' was present on the command line, this prints the parser's help to cout and returns true.
		@param printHelpIfEmpty If true, then if the app was invoked with no arguments, then the help message will be printed and this method will return true.
	*/
	[[nodiscard]] bool checkForHelpFlag (bool printHelpIfEmpty = true) const;

	/** Returns true if the parser contains a given option.
		@param argOrFlags The argument flags as they were specified when you called addArgument().
		@param includeDefaults If the specified argument was not specified on the command line, but you called addArgument() with a non-empty default string, then this function will return true if this option is true. If this option is false, then this function will only return true if an argument was specified on the command line.
	*/
	[[nodiscard]] bool containsOption (const String& argOrFlags, bool includeDefaults = false) const;

private:

	juce::ArgumentList argList;

	struct Argument final
	{
		String			  argOrFlags;
		String			  help;
		String			  defaultValue;
		bool			  required;
		juce::StringArray options;
	};

	juce::Array<Argument> args;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArgParser)
};

}  // namespace lemons
