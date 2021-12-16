#include <lemons_cryptography/lemons_cryptography.h>

void print_help()
{
	std::cout << "[--help|-h]" << std::endl
	          << "[--file|-f <filePath>]" << std::endl
	          << "[--bits|-b <numBits>]" << std::endl;
}


int main (int argc, char** argv)
{
	juce::ArgumentList args (argc, argv);

	if (args.containsOption ("--help|-h"))
	{
		print_help();
		return EXIT_SUCCESS;
	}

	if (! args.containsOption ("--file|-f"))
	{
		print_help();
		std::cout << "Option [--file|-f] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	const auto numBits = [&]() -> int
	{
		if (args.containsOption ("--bits|-b"))
			return args.getValueForOption ("--bits|-b").getIntValue();

		return 128;
	}();

	lemons::crypto::generateKeyPair (args.getFileForOption ("--file|-f"), numBits);

	return EXIT_SUCCESS;
}
