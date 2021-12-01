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


[[nodiscard]] bool executeUnitTests (juce::File logOutput, juce::int64 seed,
                                     const String& singleTestName = {}, const String& categoryName = {});

[[nodiscard]] bool executeAllTests (const juce::ArgumentList& args);

}  // namespace lemons::tests
