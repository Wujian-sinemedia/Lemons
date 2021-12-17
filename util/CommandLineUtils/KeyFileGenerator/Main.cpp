#include <juce_product_unlocking/juce_product_unlocking.h>
#include <lemons_core/lemons_core.h>


int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--file|-f", true, "The path to the output file");
	args.addArgument ("--app|-a", true, "The app name");
	args.addArgument ("--email|-e", true, "The user's email address");
	args.addArgument ("--name|-n", true, "The username");
	args.addArgument ("--machine|-m", true, "The machine ID(s)");
	args.addArgument ("--key|-k", true, "The public RSA key");
	args.addArgument ("--time|-t", false, "ISO8601-formatted string representing the time the license expires");

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	const auto file = args.getFilepathForOption ("--file|-f");

	file.deleteFile();

	const auto appName   = args["--app|-a"];
	const auto email     = args["--email|-e"];
	const auto username  = args["--name|-n"];
	const auto machineID = args["--machine|-m"];

	const juce::RSAKey privateKey { args["--key|-k"] };

	const auto string = [&]() -> juce::String
	{
		if (args.containsOption ("--time|-t"))
		{
			const auto expiryTime = juce::Time::fromISO8601 (args["--time|-t"]);

			return juce::KeyGeneration::generateExpiringKeyFile (appName, email, username, machineID, expiryTime, privateKey);
		}

		return juce::KeyGeneration::generateKeyFile (appName, email, username, machineID, privateKey);
	}();

	file.replaceWithText (string);

	return EXIT_SUCCESS;
}
