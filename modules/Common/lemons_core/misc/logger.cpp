namespace lemons
{

Logger::Logger (File outputFile)
{
	juce::Logger::setCurrentLogger (this);

	if (outputFile.getFullPathName().isEmpty())
	{
		writeToFile = false;
	}
	else
	{
		writeToFile   = true;
		fileToWriteTo = outputFile;
	}

	logMessage ("Lemons v" + Version::lemonsVersion().toString());
}

Logger::~Logger()
{
	juce::Logger::setCurrentLogger (nullptr);

	if (writeToFile)
	{
		std::cout << "Writing log output to file: " << fileToWriteTo.getFullPathName() << std::endl;
		fileToWriteTo.replaceWithText (loggedMessages.joinIntoString (" \r\n"));
	}
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

}  // namespace lemons
