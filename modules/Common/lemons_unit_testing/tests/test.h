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
    
    [[nodiscard]] static Intensity getTestingIntensityLevel();
    
    static void setGlobalTestingIntensityLevel (Intensity intensityLevel);
    
    [[nodiscard]] static bool testingIntensityIsLow();
    [[nodiscard]] static bool testingIntensityIsMedium();
    [[nodiscard]] static bool testingIntensityIsHigh();

protected:
	void logImportantMessage (const String& message);
    
    struct Subtest
    {
        explicit Subtest (const String& name, Test& t);
        ~Subtest();
        
    private:
        Test& test;
    };
    
    const Subtest beginSubtest (const String& name);
    
private:
    friend struct Subtest;
    
    void beginSubtestInternal (const String& name);
    void endSubtest();
    
    static constexpr auto indentSize = 4;
    String importantMsgIndent { "  " };
    
    static Intensity intensity;
};

}  // namespace lemons::tests
