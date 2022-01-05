/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

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

bool Runner::runTest (const String& testName, juce::int64 seed)
{
	if (testName.isEmpty())
	{
		std::cout << "Test name cannot be empty!" << std::endl;
		return false;
	}

	if (auto* test = [&testName]() -> juce::UnitTest*
	    {
		    for (auto* t : juce::UnitTest::getAllTests())
			    if (t->getName() == testName)
				    return t;

		    return nullptr;
	    }())
	{
		runTests ({ test }, seed);
		return ! hadAnyFailures();
	}

	std::cout << "Test " << testName << " could not be found!" << std::endl;
	return false;
}

void Runner::logMessage (const juce::String& message)
{
	juce::Logger::writeToLog (message);
}


struct RAII_FileLogger : public juce::FileLogger
{
	explicit RAII_FileLogger (File output)
	    : FileLogger (output, "", 0)
	{
		juce::Logger::setCurrentLogger (this);
	}

	~RAII_FileLogger()
	{
		juce::Logger::setCurrentLogger (nullptr);
	}
};


bool executeUnitTests (Intensity intensityLevel, const File& logOutput, juce::int64 seed,
                       const String& singleTestName, const String& categoryName)
{
	jassert (! (singleTestName.isNotEmpty() && categoryName.isNotEmpty()));

	Test::setGlobalTestingIntensityLevel (intensityLevel);

	RAII_FileLogger logger { logOutput };

	Runner runner;

#if LEMONS_GUI_UNIT_TESTS
	const juce::ScopedJuceInitialiser_GUI guiInit;
#endif

	if (Test::testingIntensityIsLow())
		logger.writeToLog ("Testing intensity - LOW");
	else if (Test::testingIntensityIsMedium())
		logger.writeToLog ("Testing intensity - MEDIUM");
	else
		logger.writeToLog ("Testing intensity - HIGH");

	if (singleTestName.isNotEmpty())
		return runner.runTest (singleTestName);

	if (! categoryName.isEmpty())
	{
		runner.runTestsInCategory (categoryName, seed);
		return ! runner.hadAnyFailures();
	}

	runner.runAllTests (seed);
	return ! runner.hadAnyFailures();
}

}  // namespace lemons::tests
