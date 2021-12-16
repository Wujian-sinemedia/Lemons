#include <lemons_cryptography/lemons_cryptography.h>
#include <lemons_core/lemons_core.h>


int main (int argc, char** argv)
{
    lemons::ArgParser args {argc, argv};
    
    args.addArgument ("--file|-f", true,  "Path to the output file");
    args.addArgument ("--bits|-b", false, "Number of bits for the generated keys");
    
    if (args.checkForHelpFlag())
        return EXIT_SUCCESS;
    
    if (! args.checkForRequiredArgs())
        return EXIT_FAILURE;
    
	const auto numBits = [&]() -> int
	{
		if (args.containsOption ("--bits|-b"))
			return args["--bits|-b"].getIntValue();

		return 128;
	}();

	lemons::crypto::generateKeyPair (args.getFilepathForOption ("--file|-f"), numBits);

	return EXIT_SUCCESS;
}
