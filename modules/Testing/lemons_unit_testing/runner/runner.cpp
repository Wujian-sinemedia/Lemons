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


void printUnitTestHelp()
{
	std::cout << " [--help|-h]" << std::endl
	          << " [--list-categories|-l]" << std::endl
	          << " [--category|-c <categoryName>]" << std::endl
	          << " [--test|-t <testName>]" << std::endl
	          << " [--file|-f <logFile>]" << std::endl
	          << " [--seed|-s <seedValue>]" << std::endl
	          << " [--intensity|-i <intensityLevel>]" << std::endl;
}


bool executeUnitTests (Intensity intensityLevel, File logOutput, juce::int64 seed,
                       const String& singleTestName, const String& categoryName)
{
	jassert (! (! singleTestName.isEmpty() && ! categoryName.isEmpty()));

	Test::setGlobalTestingIntensityLevel (intensityLevel);
    
    juce::FileLogger logger { logOutput, "", 0 };
    
    juce::Logger::setCurrentLogger (&logger);
    
    const auto atExit = [&]()
    {
        juce::Logger::setCurrentLogger (nullptr);
    };

	Runner runner;

#if LEMONS_GUI_UNIT_TESTS
	// set up message manager, etc...
#endif

	if (Test::testingIntensityIsLow())
		logger.writeToLog ("Testing intensity - LOW");
	else if (Test::testingIntensityIsMedium())
		logger.writeToLog ("Testing intensity - MEDIUM");
	else
		logger.writeToLog ("Testing intensity - HIGH");

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
            atExit();
			return ! runner.hadAnyFailures();
		}
        
        logger.writeToLog ("Test " + singleTestName + " not found!");
        atExit();
		return false;
	}

	if (! categoryName.isEmpty())
	{
		runner.runTestsInCategory (categoryName, seed);
        atExit();
		return ! runner.hadAnyFailures();
	}

	runner.runAllTests (seed);
    atExit();
	return ! runner.hadAnyFailures();
}

bool executeUnitTests (const juce::ArgumentList& args)
{
	if (args.containsOption ("--help|-h"))
	{
		printUnitTestHelp();
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
		if (! args.containsOption ("--seed|-s"))
			return juce::Random::getSystemRandom().nextInt64();

		const auto seedValueString = args.getValueForOption ("--seed|-s");

		if (seedValueString.startsWith ("0x"))
			return seedValueString.getHexValue64();

		return seedValueString.getLargeIntValue();
	}();

	const auto intensity = [&args]() -> Intensity
	{
		if (! args.containsOption ("--intensity|-i"))
			return Intensity::Medium;

		const auto intensityString = args.getValueForOption ("--intensity|-i");

		if (intensityString.equalsIgnoreCase ("high") || intensityString.getIntValue() == 2)
			return Intensity::High;

		if (intensityString.equalsIgnoreCase ("low") || intensityString.getIntValue() == 0)
			return Intensity::Low;

		return Intensity::Medium;
	}();

	return executeUnitTests (intensity, logFile, randomSeed,
	                         args.getValueForOption ("--test|-t"),
	                         args.getValueForOption ("--category|-c"));
}

}  // namespace lemons::tests
