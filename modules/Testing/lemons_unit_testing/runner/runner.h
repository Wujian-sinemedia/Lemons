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

using juce::File;

class Runner : public juce::UnitTestRunner
{
public:
	bool hadAnyFailures() const;

private:
	void logMessage (const juce::String& message) final;
};


[[nodiscard]] bool executeUnitTests (Intensity  intensityLevel = Intensity::Medium,
                                     const File& logOutput = {}, juce::int64 seed = juce::Random::getSystemRandom().nextInt64(),
                                     const String& singleTestName = {}, const String& categoryName = {});

}  // namespace lemons::tests
