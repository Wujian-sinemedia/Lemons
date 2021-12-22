#pragma once

namespace lemons::dsp::psola
{

template <typename SampleType>
class Shifter final
{
public:

	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	void setPitch (int pitchHz, double samplerate);

	void getSamples (AudioBuffer<SampleType>& output);

	void getSamples (SampleType* output, int numSamples);

	[[nodiscard]] SampleType getNextSample();

	void releaseResources();

private:
	friend class Analyzer<SampleType>;

	struct Grain final
	{
		using AnalysisGrain = typename Analyzer<SampleType>::Grain;

		explicit Grain (const AnalysisGrain& analysisGrainToUse, int numSilentSamplesFirst);

		[[nodiscard]] SampleType getNextSample();

		//        [[nodiscard]] bool isActive() const;

		const AnalysisGrain& analysisGrain;

		int zeroesLeft { 0 };

		int sampleIdx { 0 };
	};

	void startNewGrain();

	void newBlockStarting();

	Analyzer<SampleType>& analyzer;

	float targetPeriod { 0.f };

	int samplesToNextGrain { 0 };

	int placeInBlock { 0 };

	Array<Grain> grains;
};

}  // namespace lemons::dsp::psola
