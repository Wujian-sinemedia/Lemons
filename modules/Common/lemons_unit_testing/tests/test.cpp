namespace lemons::tests
{

void Test::logImportantMessage (const String& message)
{
	logMessage (" ");
	logMessage (String ("  ---  ") + message + "  ---  ");
	logMessage (" ");
}

Intensity Test::getTestingIntensityLevel()
{
    return intensity;
}

void Test::setGlobalTestingIntensityLevel (Intensity intensityLevel)
{
    intensity = intensityLevel;
}

bool Test::testingIntensityIsLow()
{
    return intensity == Intensity::Low;
}

bool Test::testingIntensityIsMedium()
{
    return intensity == Intensity::Medium;
}

bool Test::testingIntensityIsHigh()
{
    return intensity == Intensity::High;
}

Intensity Test::intensity { Intensity::Medium };

}  // namespace lemons::tests
