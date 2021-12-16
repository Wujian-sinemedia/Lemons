#include <juce_product_unlocking/juce_product_unlocking.h>


void print_help()
{
	std::cout << "[--help|-h]" << std::endl
	          << "[--file|-f <filePath>]" << std::endl
	          << "[--app|-a <appName>]" << std::endl
	          << "[--email|-e <userEmail>]" << std::endl
	          << "[--name|-n <userName>]" << std::endl
	          << "[--machine|-m <machineID>]" << std::endl
	          << "[--key|-k <privateKey>]" << std::endl
	          << "[--time|-t <expiryTime>]" << std::endl;
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

	if (! args.containsOption ("--app|-a"))
	{
		print_help();
		std::cout << "Option [--app|-a] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	if (! args.containsOption ("--email|-e"))
	{
		print_help();
		std::cout << "Option [--email|-e] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	if (! args.containsOption ("--name|-n"))
	{
		print_help();
		std::cout << "Option [--name|-n] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	if (! args.containsOption ("--machine|-m"))
	{
		print_help();
		std::cout << "Option [--machine|-m] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	if (! args.containsOption ("--key|-k"))
	{
		print_help();
		std::cout << "Option [--key|-k] is required!" << std::endl;
		return EXIT_FAILURE;
	}

	const auto file = args.getFileForOption ("--file|-f");

	file.deleteFile();

	const auto appName   = args.getValueForOption ("--app|-a");
	const auto email     = args.getValueForOption ("--email|-e");
	const auto username  = args.getValueForOption ("--name|-n");
	const auto machineID = args.getValueForOption ("--machine|-m");

	const juce::RSAKey privateKey { args.getValueForOption ("--key|-k") };

	const auto string = [&]() -> juce::String
	{
		if (args.containsOption ("--time|-t"))
		{
			const auto expiryTime = juce::Time::fromISO8601 (args.getValueForOption ("--time|-t"));

			return juce::KeyGeneration::generateExpiringKeyFile (appName, email, username, machineID, expiryTime, privateKey);
		}

		return juce::KeyGeneration::generateKeyFile (appName, email, username, machineID, privateKey);
	}();

	file.replaceWithText (string);

	return EXIT_SUCCESS;
}
