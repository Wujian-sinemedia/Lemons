namespace lemons::console
{

Logger::Logger (juce::File outputFile)
{
    juce::Logger::setCurrentLogger (this);
    
    if (outputFile.getFullPathName().isEmpty())
    {
        writeToFile = false;
    }
    else
    {
        writeToFile = true;
        fileToWriteTo = outputFile;
        
        logMessage ("Writing log to file path: " + fileToWriteTo.getFullPathName());
    }
}

Logger::~Logger()
{
    juce::Logger::setCurrentLogger (nullptr);
    
    if (writeToFile)
        fileToWriteTo.replaceWithText (loggedMessages.joinIntoString (" \r\n"));
}

void Logger::logMessage (const juce::String& message)
{
    std::cout << message << std::endl;
    
#if JUCE_WINDOWS
    juce::Logger::outputDebugString (message);
#endif
    
    if (writeToFile)
        loggedMessages.add (message);
}

}

