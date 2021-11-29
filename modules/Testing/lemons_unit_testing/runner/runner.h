#pragma once

namespace lemons::tests
{

class ConsoleLogger : public juce::Logger
{
	void logMessage (const juce::String& message) final;
};


struct ConsoleUnitTestRunner : public juce::UnitTestRunner
{
	void logMessage (const juce::String& message) final;
};


bool executeAllTests (const juce::ArgumentList& args);

}  // namespace lemons::tests
