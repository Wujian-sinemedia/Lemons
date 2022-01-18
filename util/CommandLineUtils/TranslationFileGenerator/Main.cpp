#include <lemons_core/lemons_core.h>

juce::StringArray getDefaultLanguageCodes()
{
	return { "ara", "chi", "fre", "ger", "ita", "por", "rus", "spa" };
}

int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--dir|-d", true, "The path to the root of the source tree to search for translations");
	args.addArgument ("--out|-o", true, "The path to the output directory for the translation files");
	args.addArgument ("--prefix|-p", false, "The prefix added to all generated translation file names", "trans_");
	args.addArgument ("--extension|-x", false, "The file extension used for all generated translation files", ".txt");

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	std::cout << "Generating translation files..." << std::endl;

	lemons::locale::generateTranslationFiles (args.getFilepathForOption ("--dir|-d"),
	                                          getDefaultLanguageCodes(),
	                                          args.getFilepathForOption ("--out|-o"),
	                                          args["--prefix|-p"], args["--extension|-x"]);

	return EXIT_SUCCESS;
}
