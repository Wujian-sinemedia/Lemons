#include <lemons_core/lemons_core.h>

namespace lemons::tests
{

bool Runner::hadAnyFailures() const
{
	for (int i = 0; i < getNumResults(); ++i)
		if (getResult (i)->failures > 0)
			return true;

	return false;
}

void Runner::logMessage (const juce::String& message)
{
	juce::Logger::writeToLog (message);
}


bool executeUnitTests (juce::File logOutput, juce::int64 seed,
                       const String& singleTestName, const String& categoryName)
{
	jassert (! (! singleTestName.isEmpty() && ! categoryName.isEmpty()));

	Logger logger { logOutput };
	Runner runner;

#if LEMONS_GUI_UNIT_TESTS
	// set up message manager, etc...
#endif

	if (! singleTestName.isEmpty())
	{
		if (auto* test = [&singleTestName]() -> juce::UnitTest*
		    {
			    for (auto* t : juce::UnitTest::getAllTests())
				    if (t->getName() == singleTestName)
					    return t;

			    return nullptr;
		    }())
		{
			runner.runTests ({ test }, seed);
			return ! runner.hadAnyFailures();
		}

		return false;
	}

	if (! categoryName.isEmpty())
	{
		runner.runTestsInCategory (categoryName, seed);
		return ! runner.hadAnyFailures();
	}

	runner.runAllTests (seed);
	return ! runner.hadAnyFailures();
}


bool executeUnitTests (const juce::ArgumentList& args)
{
	if (args.containsOption ("--help|-h"))
	{
		std::cout << " [--help|-h] [--list-categories|-l] [--category|-c <categoryName>] [--test|-t <testName>] [--file|-f <logFile>] [--seed|-s <seedValue>]" << std::endl;
		return true;
	}

	if (args.containsOption ("--list-categories|-l"))
	{
		for (const auto& category : juce::UnitTest::getAllCategories())
			std::cout << category << std::endl;

		return true;
	}

	const auto logFile = [&args]() -> juce::File
	{
		if (! args.containsOption ("--file|-f"))
			return {};

		const auto path = args.getValueForOption ("--file|-f");

		if (juce::File::isAbsolutePath (path))
			return { path };

		return juce::File::getCurrentWorkingDirectory().getChildFile (path);
	}();

	const auto randomSeed = [&args]() -> juce::int64
	{
		if (args.containsOption ("--seed|-s"))
		{
			const auto seedValueString = args.getValueForOption ("--seed|-s");

			if (seedValueString.startsWith ("0x"))
				return seedValueString.getHexValue64();

			return seedValueString.getLargeIntValue();
		}

		return juce::Random::getSystemRandom().nextInt64();
	}();

	return executeUnitTests (logFile, randomSeed,
	                         args.getValueForOption ("--test|-t"),
	                         args.getValueForOption ("--category|-c"));
}

}  // namespace lemons::tests
