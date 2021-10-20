
#pragma once

namespace lemons::dsp::psola
{
/** Respaces a stream of analysis grains identified by an Analyzer object into a repitched stream of audio.
    @see Analyzer
 */
template <typename SampleType>
class Shifter
{
public:
	using AudioBuffer = juce::AudioBuffer<SampleType>;

	/** Constructs a shifter. */
	Shifter (Analyzer<SampleType>& parentAnalyzer);

	/** Sets the desired output pitch of the repitched audio. */
	void setPitch (float desiredFrequency, double samplerate);

	/** Processes a stream of samples. This can only be used to synthesize one channel of audio at a time. */
	void getSamples (AudioBuffer& output, int channel = 0);

	/** Processes a stream of samples. */
	void getSamples (SampleType* outputSamples, int numSamples);

	/** Returns the next repitched sample value. */
	SampleType getNextSample();

private:
	/*--------------------------------------------------------*/

	class Grain
	{
	public:
		using Storage = AnalysisGrainStorage<SampleType>;

		Grain (const Storage& storageToUse);

		bool isActive() const;

		void startNewGrain (int start, int length);

		SampleType getNextSample();

	private:
		const Storage& storage;

		bool active { false };

		int startIndex { 0 };
		int grainLength { 0 };

		int currentTick { 0 };
	};

	/*--------------------------------------------------------*/

	void   startNewGrain();
	Grain* getAvailableGrain() const;
	bool   areAnyGrainsActive() const;

	Analyzer<SampleType>&                   analyzer;
	const AnalysisGrainStorage<SampleType>& storage { analyzer.grainStorage };

	juce::OwnedArray<Grain> grains;

	int desiredPeriod { 0 };
	int samplesToNextGrain { 0 };
	int currentSample { 0 };

	events::Listener l;
};


}  // namespace lemons::dsp::psola
