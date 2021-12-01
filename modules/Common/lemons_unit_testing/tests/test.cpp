namespace lemons::tests
{

void Test::logImportantMessage (const String& message)
{
	logMessage (importantMsgIndent + " ---  " + message);
}

void Test::beginSubtestInternal (const String& name)
{
    logMessage (importantMsgIndent + " *****  " + name);
    
    for (auto i = 0; i < indentSize; ++i)
        importantMsgIndent.append (" ", 1);
}

void Test::endSubtest()
{
    for (auto i = 0; i < indentSize && importantMsgIndent.length() > 1; ++i)
        importantMsgIndent = importantMsgIndent.substring (1);
}

const Test::Subtest Test::beginSubtest (const String& name)
{
    return Subtest { name, *this };
}

Test::Subtest::Subtest (const String& name, Test& t)
: test(t)
{
    test.beginSubtestInternal (name);
}

Test::Subtest::~Subtest()
{
    test.endSubtest();
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
