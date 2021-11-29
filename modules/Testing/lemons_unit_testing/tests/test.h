#pragma once

#include <juce_core/juce_core.h>


namespace lemons::tests
{

class Test : public juce::UnitTest
{
public:
    using juce::UnitTest::UnitTest;
    
protected:
    using String = juce::String;
    
    void logImportantMessage (const String& message);
    
    const String& getLastImportantMessage() const;
    
    static const std::vector<double>& getTestingSamplerates();
    
private:
    String lastImportantMessage;
};

}
