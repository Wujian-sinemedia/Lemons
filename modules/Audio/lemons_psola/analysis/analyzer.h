#pragma once


namespace lemons::dsp::psola
{

template <typename SampleType>
class Shifter;

template <typename SampleType>
class Analyzer final
{
public:
	explicit Analyzer (int minFreqHz = 60);

	void analyzeInput (const AudioBuffer<SampleType>& inputAudio);

	void analyzeInput (const SampleType* inputAudio, int numSamples);

	[[nodiscard]] int getLatencySamples() const noexcept;

	int setSamplerate (double newSamplerate);

	int setMinInputFreq (int minFreqHz);

	void releaseResources();

private:
	friend class Shifter<SampleType>;

	void newBlockStarting();

	inline int latencyChanged();

	void makeWindow();

	/*-----------------------------------------------------------------------------------*/

	struct Grain final
	{
		int origStartIndex { 0 };

		int grainSize { 0 };

		AudioBuffer<SampleType> samples;
	};

	const Grain* getClosestGrain (int placeInBlock) const;

	void registerShifter (Shifter<SampleType>* shifter);

	/*-----------------------------------------------------------------------------------*/

	PitchDetector<SampleType> pitchDetector;
	PeakFinder<SampleType>    peakFinder;

	float currentPeriod { 0.f };

	Array<SampleType> window;

	Array<Grain> grains;

	int lastBlocksize { 0 };

	Array<Shifter<SampleType>*> shifters;
};

}  // namespace lemons::dsp::psola
