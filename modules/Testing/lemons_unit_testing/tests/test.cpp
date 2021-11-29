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

}  // namespace lemons::tests
