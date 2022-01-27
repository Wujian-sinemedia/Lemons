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
	for (auto i = 0; i < getNumResults(); ++i)
		if (getResult (i)->failures > 0)
			return true;

	return false;
}

juce::StringArray Runner::getFailedTestNames() const
{
	juce::StringArray names;

	for (auto i = 0; i < getNumResults(); ++i)
	{
		const auto* result = getResult (i);

		if (result->failures > 0)
			names.add (result->unitTestName);
	}

	names.trim();
	names.removeEmptyStrings();
	names.removeDuplicates (true);

	return names;
}

bool Runner::runTest (const String& testName, juce::int64 seed)
{
	if (testName.isEmpty())
	{
		std::cout << "Test name cannot be empty!" << std::endl;
		return false;
	}

	if (auto* test = getUnitTestWithName (testName))
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


juce::UnitTest* getUnitTestWithName (const String& name)
{
	for (auto* t : juce::UnitTest::getAllTests())
		if (t->getName() == name)
			return t;

	return nullptr;
}


bool executeUnitTests (Intensity intensityLevel, const File& logOutput, juce::int64 seed,
					   const String& singleTestName, const String& categoryName,
					   bool rerunFailed)
{
	jassert (! (singleTestName.isNotEmpty() && categoryName.isNotEmpty()));

	Test::setGlobalTestingIntensityLevel (intensityLevel);

	struct RAII_FileLogger final : public juce::FileLogger
	{
		explicit RAII_FileLogger (const File& output)
			: FileLogger (output, "", 0)
		{
			juce::Logger::setCurrentLogger (this);
		}

		~RAII_FileLogger()
		{
			juce::Logger::setCurrentLogger (nullptr);
		}
	};

	RAII_FileLogger logger { logOutput };

	Runner runner;

	const juce::ScopedJuceInitialiser_GUI guiInit;

	if (Test::testingIntensityIsLow())
		logger.writeToLog ("Testing intensity - LOW");
	else if (Test::testingIntensityIsMedium())
		logger.writeToLog ("Testing intensity - MEDIUM");
	else
		logger.writeToLog ("Testing intensity - HIGH");

	const auto result = [&singleTestName, &categoryName, &runner, seed]
	{
		if (singleTestName.isNotEmpty())
			return runner.runTest (singleTestName, seed);

		if (categoryName.isNotEmpty())
			runner.runTestsInCategory (categoryName, seed);
		else
			runner.runAllTests (seed);

		return ! runner.hadAnyFailures();
	}();

	if (result)
		return true;

	const auto failedTestNames = runner.getFailedTestNames();

	logger.writeToLog ("The following tests failed: " + failedTestNames.joinIntoString (", "));

	if (! rerunFailed)
		return false;

	juce::Array<juce::UnitTest*> failedTests;

	for (const auto& name : failedTestNames)
		failedTests.add (getUnitTestWithName (name));

	runner.runTests (failedTests, seed);

	if (! runner.hadAnyFailures())
		return true;

	logger.writeToLog ("The following tests failed again: " + runner.getFailedTestNames().joinIntoString (", "));

	return false;
}

}  // namespace lemons::tests
