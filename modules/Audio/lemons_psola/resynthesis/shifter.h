/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

namespace lemons::dsp::psola
{

template <typename SampleType>
class Shifter final
{
public:

	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	~Shifter();

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

		~Grain();

		[[nodiscard]] SampleType getNextSample();

		[[nodiscard]] bool isActive() const;

		void clearGrain();

		AnalysisGrain* analysisGrain;

		int sampleIdx { 0 };
	};

	void startNewGrain();

	void newBlockStarting();

	[[nodiscard]] Grain& getGrainToStart();

	Analyzer<SampleType>& analyzer;

	float targetPeriod { 0.f };

	int samplesToNextGrain { 0 };

	int placeInBlock { 0 };

	int targetPitchHz { 0 };

	juce::OwnedArray<Grain> grains;
};

}  // namespace lemons::dsp::psola


/*---------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename SampleType>
struct PsolaTests : public DspTest
{
	PsolaTests();

private:
	void runTest() final;

	dsp::psola::Analyzer<SampleType> analyzer;
	dsp::psola::Shifter<SampleType>  shifter { analyzer };

	dsp::osc::Sine<SampleType> osc;
	AudioBuffer<SampleType>    origAudio, shiftedAudio;

	dsp::PitchDetector<SampleType> detector;
};

LEMONS_CREATE_DSP_TEST (PsolaTests)

}  // namespace lemons::tests

#endif
