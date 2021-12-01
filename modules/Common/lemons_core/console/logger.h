#pragma once

namespace lemons::console
{

class Logger : public juce::Logger
{
public:
    Logger (juce::File outputFile = {});
    
    ~Logger() override;
    
private:
    void logMessage (const juce::String& message) final;
    
    juce::StringArray loggedMessages;
    
    juce::File fileToWriteTo;
    
    bool writeToFile { false };
};

}
