namespace lemons::tests
{

void Test::logImportantMessage (const String& message)
{
	logMessage (" ");
	logMessage (String ("  ---  ") + message + "  ---  ");
	logMessage (" ");
}

}  // namespace lemons::tests
