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

#pragma once

namespace lemons::tests
{

class Runner : public juce::UnitTestRunner
{
public:
	bool hadAnyFailures() const;

private:
	void logMessage (const juce::String& message) final;
};


[[nodiscard]] bool executeUnitTests (Intensity  intensityLevel = Intensity::Medium,
                                     juce::File logOutput = {}, juce::int64 seed = juce::Random::getSystemRandom().nextInt64(),
                                     const String& singleTestName = {}, const String& categoryName = {});

[[nodiscard]] bool executeUnitTests (const juce::ArgumentList& args);

}  // namespace lemons::tests
