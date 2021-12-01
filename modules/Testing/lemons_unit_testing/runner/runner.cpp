namespace lemons::tests
{

class ConsoleLogger : public juce::Logger
{
public:
	ConsoleLogger()
	{
		juce::Logger::setCurrentLogger (this);
	}

	~ConsoleLogger() override
	{
		juce::Logger::setCurrentLogger (nullptr);
	}

private:
	void logMessage (const juce::String& message) final
	{
		std::cout << message << std::endl;

#if JUCE_WINDOWS
		juce::Logger::outputDebugString (message);
#endif
	}
};


class ConsoleUnitTestRunner : public juce::UnitTestRunner
{
public:
	bool hadAnyFailures() const
	{
		for (int i = 0; i < getNumResults(); ++i)
			if (getResult (i)->failures > 0)
				return true;

		return false;
	}

private:
	void logMessage (const juce::String& message) final
	{
		juce::Logger::writeToLog (message);
	}
};


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

	ConsoleUnitTestRunner runner;
	ConsoleLogger         logger;

#if LEMONS_GUI_UNIT_TESTS
	// set up message manager, etc...
#endif

	if (args.containsOption ("--category"))
		runner.runTestsInCategory (args.getValueForOption ("--category"), seed);
	else if (args.containsOption ("-c"))
		runner.runTestsInCategory (args.getValueForOption ("-c"), seed);
	else
		runner.runAllTests (seed);

	return ! runner.hadAnyFailures();
}

}  // namespace lemons::tests
