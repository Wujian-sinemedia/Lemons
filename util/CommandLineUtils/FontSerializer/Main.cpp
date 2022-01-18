#include <lemons_gui/lemons_gui.h>


int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--file|-f", true, "The path to the output file");
	args.addArgument ("--name|-n", true, "The name of the font to serialize");
	args.addArgument ("--size|-s", false, "The size of the font in pixels");
	args.addArgument ("--chars|-c", false, "The maximum number of characters from the font to serialize", "127");
	args.addArgument ("--default|-d", false, "The default character for the font", " ");
	args.addArgument ("--bold|-b", false, "Include to make the font bold");
	args.addArgument ("--italic|-i", false, "Include to make the font italic");
	args.addArgument ("--underline|-u", false, "Include to make the font underlined");
	args.addArgument ("--list|-l", false, "List all fonts on the system and exit");

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (args.containsOption ("--list|-l"))
	{
		for (const auto& name : juce::Font::findAllTypefaceNames())
			std::cout << name << std::endl;

		return EXIT_SUCCESS;
	}

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	const juce::ScopedJuceInitialiser_GUI init;

	int style = juce::Font::plain;

	if (args.containsOption ("--bold|-b"))
		style |= juce::Font::bold;

	if (args.containsOption ("--italic|-i"))
		style |= juce::Font::italic;

	if (args.containsOption ("--underline|-u"))
		style |= juce::Font::underlined;

	const juce::Font font { args["--name|-n"],
		                    args.getArgumentAsType<float> ("--size|-s"),
		                    style };

	const auto res = lemons::files::saveFont (font,
	                                          args.getFilepathForOption ("--file|-f"),
	                                          args.getArgumentAsType<int> ("--chars|-c"),
	                                          args.getArgumentAsType<juce::juce_wchar> ("--default|-d"));

	if (res)
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}
