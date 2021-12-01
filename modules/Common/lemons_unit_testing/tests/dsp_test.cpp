namespace lemons::tests
{

DspTest::DspTest (const String& testName)
    : Test (testName, "DSP")
{
}

DspTest::DspTest (const String& testName, const String& testCategory)
    : Test (testName, testCategory)
{
}

template <typename SampleType>
String DspTest::getDspTestName (const String& name)
{
	if constexpr (std::is_same_v<SampleType, float>)
		return name + " (float)";
	else
		return name + " (double)";
}

template String DspTest::getDspTestName<float> (const String&);
template String DspTest::getDspTestName<double> (const String&);

void DspTest::fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents)
{
	buffer.clear();

	auto rand = getRandom();

	for (int i = 0; i < numEvents; ++i)
		buffer.addEvent (juce::MidiMessage::controllerEvent (1, rand.nextInt (128), rand.nextInt (128)),
		                 i);
}

template <typename SampleType>
void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer)
{
	auto rand = getRandom();

	for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
	{
		auto* samples = buffer.getWritePointer (chan);

		for (int s = 0; s < buffer.getNumSamples(); ++s)
			samples[s] = static_cast<SampleType> (juce::jmap (rand.nextFloat(), -1.f, 1.f));
	}
}

template void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<float>&);
template void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<double>&);


const std::vector<double>& DspTest::getTestingSamplerates()
{
	static const std::vector<double> samplerates { 44100., 44800., 96000. };
	return samplerates;
}

const std::vector<int>& DspTest::getTestingBlockSizes()
{
	static const std::vector<int> blocksizes { 256, 513, 1021, 2048 };
	return blocksizes;
}

}  // namespace lemons::tests
