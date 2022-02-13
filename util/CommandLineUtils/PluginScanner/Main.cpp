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

#include <lemons_plugin/lemons_plugin.h>

int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--paths|-p", true, "Semicolon- or comma-separated list of paths to search for plugins");
	args.addArgument ("--file|-f", true, "Output file to write the list of plugins to");
	args.addArgument ("--blacklist|-b", true, "Output file to write the list of blacklisted plugins to");
	args.addArgument ("--sort|-s", false, "Sort method for the plugin tree", "Manufacturer", { "Category", "Manufacturer", "Format" });
	args.addArgument ("--root-category|-r", false, "Name of the root category of the plugin tree", "Plugins");
	args.addArgument ("--file-format|-m", false, "File format of the output file", "JSON", { "JSON", "XML", "Opaque" });

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	const auto sortMethod = [method = args["--sort|-s"]]
	{
		using Method = lemons::plugin::scanning::Category::SortMethod;

		if (method.equalsIgnoreCase ("Category"))
			return Method::sortByCategory;

		if (method.equalsIgnoreCase ("Manufacturer"))
			return Method::sortByManufacturer;

		jassert (method.equalsIgnoreCase ("Format"));

		return Method::sortByFormat;
	}();

	juce::ThreadPool pool;

	const auto rootCategory = lemons::plugin::scanning::scanDirectory ({ args["--paths|-p"] },
																	   args.getFilepathForOption ("--blacklist|-b"),
																	   sortMethod,
																	   args["--root-category|-r"],
																	   &pool);

	const auto format = args["--file-format|-m"];
	const auto file	  = args.getFilepathForOption ("--file|-f");

	using Format = lemons::files::FileType;

	if (format.equalsIgnoreCase ("JSON"))
		rootCategory->saveToFile<Format::JSON> (file);
	else if (format.equalsIgnoreCase ("XML"))
		rootCategory->saveToFile<Format::XML> (file);
	else
	{
		jassert (format.equalsIgnoreCase ("Opaque"));
		rootCategory->saveToFile<Format::Opaque> (file);
	}

	return EXIT_SUCCESS;
}
