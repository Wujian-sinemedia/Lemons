#include <lemons_core/lemons_core.h>


int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--file|-f", true, "Path to the output file");
	args.addArgument ("--bits|-b", false, "Number of bits for the generated keys", "128");

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	lemons::rsa::generateKeyPair (args.getFilepathForOption ("--file|-f"),
		args.getArgumentAsType<int> ("--bits|-b"));

	return EXIT_SUCCESS;
}
