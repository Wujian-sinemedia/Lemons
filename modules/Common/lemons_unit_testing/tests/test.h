#pragma once

#include <juce_core/juce_core.h>


namespace lemons::tests
{

enum class Intensity
{
    Low,
    Medium,
    High
};


class Test : public juce::UnitTest
{
public:
	using juce::UnitTest::UnitTest;
    
    static Intensity getTestingIntensityLevel();
    
    static void setGlobalTestingIntensityLevel (Intensity intensityLevel);
    
    static bool testingIntensityIsLow();
    static bool testingIntensityIsMedium();
    static bool testingIntensityIsHigh();

protected:
	using String = juce::String;

	void logImportantMessage (const String& message);
    
private:
    static Intensity intensity;
};

}  // namespace lemons::tests
