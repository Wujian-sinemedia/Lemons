#pragma once

namespace lemons
{

class Logger : public juce::Logger
{
public:
    Logger (File outputFile = {});
    
    ~Logger() override;
    
private:
    void logMessage (const juce::String& message) final;
    
    juce::StringArray loggedMessages;
    
    File fileToWriteTo;
    
    bool writeToFile { false };
};

}
