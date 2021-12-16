/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


namespace lemons
{

ArgParser::ArgParser (int argc, char** argv)
    : argList (argc, argv)
{
	addArgument ("--help|-h", false, "Display usage information");
}

void ArgParser::addArgument (const String& argOrFlags,
                             bool          required,
                             const String& help, const String& defaultValue)
{
	Argument newArg;
	newArg.argOrFlags   = argOrFlags;
	newArg.defaultValue = defaultValue;
	newArg.required     = required;
	newArg.help         = help;
	args.add (newArg);
}

void ArgParser::printHelp() const
{
	for (const auto& arg : args)
	{
		std::cout << "[ " << arg.argOrFlags << " ]";

		if (! arg.help.isEmpty())
			std::cout << " : " << arg.help;

		if (arg.required)
			std::cout << " [R]";

		if (! arg.defaultValue.isEmpty())
			std::cout << " - Default: " << arg.defaultValue;

		std::cout << std::endl;
	}
}

String ArgParser::operator[] (const String& argOrFlags) const
{
	if (argList.containsOption (argOrFlags))
		return argList.getValueForOption (argOrFlags);

	for (const auto& arg : args)
		if (arg.argOrFlags == argOrFlags)
			return arg.defaultValue;

	return {};
}

File ArgParser::getFilepathForOption (const String& argOrFlags) const
{
	const auto argValue = (*this)[argOrFlags];

	if (argValue.isEmpty())
		return {};

	if (File::isAbsolutePath (argValue))
		return { argValue };

	return File::getCurrentWorkingDirectory().getChildFile (argValue);
}

bool ArgParser::checkForRequiredArgs() const
{
	for (const auto& arg : args)
	{
		if (arg.required)
		{
			if (! argList.containsOption (arg.argOrFlags))
			{
				std::cout << "Argument " << arg.argOrFlags << " is required!" << std::endl;
				std::cout << "For usage help, run with --help or -h." << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool ArgParser::checkForHelpFlag() const
{
	if (argList.containsOption ("--help|-h"))
	{
		printHelp();
		return true;
	}

	return false;
}

bool ArgParser::containsOption (const String& argOrFlags, bool includeDefaults) const
{
	if (argList.containsOption (argOrFlags))
		return true;

	if (! includeDefaults)
		return false;

	for (const auto& arg : args)
		if (arg.argOrFlags == argOrFlags)
			if (! arg.defaultValue.isEmpty())
				return true;

	return false;
}

}  // namespace lemons
