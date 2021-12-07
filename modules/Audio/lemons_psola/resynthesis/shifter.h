#pragma once

namespace lemons::dsp::psola
{

template <typename SampleType>
class Shifter final
{
public:

	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	void setPitch (int pitchHz, double samplerate);

	void newBlockReceived (int numSamples);

	void getSamples (AudioBuffer<SampleType>& output);

	void getSamples (SampleType* output, int numSamples);

private:
	Analyzer<SampleType>& analyzer;

	int inptr { 0 };
	int outptr { 0 };

	int placeInInputBlock { 0 };

	int lastBlocksize { 0 };

	float targetPeriod { 0.f };
};

}  // namespace lemons::dsp::psola
