#pragma once


namespace lemons::tests
{

struct DspTest : public Test
{
	DspTest (const String& testName);

protected:
	DspTest (const String& testName, const String& testCategory);

	template <typename SampleType>
	[[nodiscard]] static String getDspTestName (const String& name);

	[[nodiscard]] static const std::vector<int>&    getTestingBlockSizes();
	[[nodiscard]] static const std::vector<double>& getTestingSamplerates();

	void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents);

	template <typename SampleType>
	void fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer);
};

}  // namespace lemons::tests
