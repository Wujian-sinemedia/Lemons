#include <lemons_unit_testing/lemons_unit_testing.h>
#include <lemons_core/lemons_core.h>


int main (int argc, char** argv)
{
	lemons::ArgParser args { argc, argv };

	args.addArgument ("--intensity|-i", false, "Testing intensity level", "Medium", { "Low", "Medium", "High" });
	args.addArgument ("--file|-f", false, "Path to the log output file");
	args.addArgument ("--seed|-s", false, "Random seed");
	args.addArgument ("--test|-t", false, "Run a single named test");
	args.addArgument ("--category|-c", false, "Run tests in a single category");
	args.addArgument ("--list|-l", false, "List all test categories");
	args.addArgument ("--list-all|-a", false, "List all test names");

	if (args.checkForHelpFlag())
		return EXIT_SUCCESS;

	if (args.containsOption ("--list|-l"))
	{
		for (const auto& category : juce::UnitTest::getAllCategories())
			std::cout << category << std::endl;

		return EXIT_SUCCESS;
	}

	if (args.containsOption ("--list-all|-a"))
	{
		for (const auto* test : juce::UnitTest::getAllTests())
			std::cout << test->getName() << std::endl;

		return EXIT_SUCCESS;
	}

	if (! args.checkForRequiredArgs())
		return EXIT_FAILURE;

	if (args.containsOption ("--test|-t") && args.containsOption ("--category|-c"))
	{
		std::cout << "Only one of --test|-t and --category|-c may be specified!" << std::endl;
		return EXIT_FAILURE;
	}

	using lemons::tests::Intensity;

	const auto intensity = [&]() -> Intensity
	{
		if (args["--intensity|-i"] == "Low")
			return Intensity::Low;

		if (args["--intensity|-i"] == "Medium")
			return Intensity::Medium;

		return Intensity::High;
	}();

	const auto seed = [&]() -> juce::int64
	{
		if (args.containsOption ("--seed|-s"))
			return args.getArgumentAsType<juce::int64> ("--seed|-s");

		return juce::Random::getSystemRandom().nextInt64();
	}();

	const auto res = lemons::tests::executeUnitTests (intensity,
	                                                  args.getFilepathForOption ("--file|-f"),
	                                                  seed,
	                                                  args["--test|-t"],
	                                                  args["--category|-c"]);

	if (res)
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}
