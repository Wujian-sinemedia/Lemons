namespace lemons::tests
{

void ConsoleLogger::logMessage (const juce::String& message)
{
	std::cout << message << std::endl;

#if JUCE_WINDOWS
	juce::Logger::outputDebugString (message);
#endif
}


void ConsoleUnitTestRunner::logMessage (const juce::String& message)
{
	juce::Logger::writeToLog (message);
}


bool executeAllTests (const juce::ArgumentList& args)
{
	if (args.containsOption ("--help|-h"))
	{
		std::cout << " [--help|-h] [--list-categories|-l] [--category|-c <category>] [--seed|-s <seed>]" << std::endl;
		return true;
	}

	if (args.containsOption ("--list-categories|-l"))
	{
		for (auto& category : juce::UnitTest::getAllCategories())
			std::cout << category << std::endl;

		return true;
	}

	ConsoleLogger logger;
	juce::Logger::setCurrentLogger (&logger);

	ConsoleUnitTestRunner runner;

	const auto seed = [&args]
	{
		if (args.containsOption ("--seed"))
		{
			const auto seedValueString = args.getValueForOption ("--seed");

			if (seedValueString.startsWith ("0x"))
				return seedValueString.getHexValue64();

			return seedValueString.getLargeIntValue();
		}

		if (args.containsOption ("-s"))
		{
			const auto seedValueString = args.getValueForOption ("-s");

			if (seedValueString.startsWith ("0x"))
				return seedValueString.getHexValue64();

			return seedValueString.getLargeIntValue();
		}

		return juce::Random::getSystemRandom().nextInt64();
	}();

	if (args.containsOption ("--category"))
		runner.runTestsInCategory (args.getValueForOption ("--category"), seed);
	else if (args.containsOption ("-c"))
		runner.runTestsInCategory (args.getValueForOption ("-c"), seed);
	else
		runner.runAllTests (seed);

	juce::Logger::setCurrentLogger (nullptr);

	for (int i = 0; i < runner.getNumResults(); ++i)
		if (runner.getResult (i)->failures > 0)
			return false;

	return true;
}

}  // namespace lemons::tests
