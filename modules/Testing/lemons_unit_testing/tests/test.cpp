namespace lemons::tests
{

template <typename SampleType>
String Test::getPrecisionString()
{
	if constexpr (std::is_same_v<SampleType, float>)
		return "Float";
	else
		return "Double";
}

template String Test::getPrecisionString<float>();
template String Test::getPrecisionString<double>();

template <typename SampleType>
String Test::getDspTestName (const String& name)
{
	return name + " (" + getPrecisionString<SampleType>() + ")";
}

template String Test::getDspTestName<float> (const String&);
template String Test::getDspTestName<double> (const String&);

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
    : test (t)
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

Intensity Test::intensity { Intensity::Low };

const std::vector<double> Test::getTestingSamplerates()
{
	if (testingIntensityIsLow())
		return { 44100. };

	if (testingIntensityIsMedium())
        return { 44100., 44800., 96000. };

	return { 36500., 44100., 55000., 96000., 180000. };
}

const std::vector<int> Test::getTestingBlockSizes()
{
	if (testingIntensityIsLow())
		return { 512 };

	if (testingIntensityIsMedium())
        return { 512, 1021, 2048 };

	return { 41, 400, 1433, 2000, 3531 };
}

int Test::getNumTestingRepetitions()
{
	if (testingIntensityIsLow())
		return 1;

	if (testingIntensityIsMedium())
		return 5;

	return 10;
}


DspTest::DspTest (const String& testName)
    : Test (testName, "DSP")
{
}

CoreTest::CoreTest (const String& testName)
    : Test (testName, "Core")
{
}

MidiTest::MidiTest (const String& testName)
    : Test (testName, "MIDI")
{
}

}  // namespace lemons::tests
