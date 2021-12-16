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


#pragma once

namespace lemons
{

class ArgParser final
{
public:

	explicit ArgParser (int argc, char** argv);

	void addArgument (const String& argOrFlags,
	                  bool          required     = false,
	                  const String& help         = {},
	                  const String& defaultValue = {});

	void printHelp() const;

	String operator[] (const String& argOrFlags) const;

	File getFilepathForOption (const String& argOrFlags) const;

	bool checkForRequiredArgs() const;

	bool checkForHelpFlag() const;

	bool containsOption (const String& argOrFlags, bool includeDefaults = false) const;

private:
	juce::ArgumentList argList;

	struct Argument
	{
		String argOrFlags;
		String help;
		String defaultValue;
		bool   required;
	};

	juce::Array<Argument> args;
};

}  // namespace lemons
