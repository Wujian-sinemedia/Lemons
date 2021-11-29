namespace lemons::tests
{

void Test::logImportantMessage (const String& message)
{
    logMessage (" ");
    logMessage (String ("  ---  ") + message + "  ---  ");
    logMessage (" ");
    
    lastImportantMessage = message;
}

const juce::String& Test::getLastImportantMessage() const
{
    return lastImportantMessage;
}

const std::vector<double>& Test::getTestingSamplerates()
{
    static const std::vector<double> samplerates { 44100., 44800., 96000. };
    return samplerates;
}

}
