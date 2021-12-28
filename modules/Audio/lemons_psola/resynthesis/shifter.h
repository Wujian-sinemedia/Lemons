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

/** A class that repitches a stream of monophonic audio using PSOLA-like techniques.
    This class is essentially a "client" of an Analyzer object, which allows multiple Shifters to be used simultaneously without requiring the expensive analysis process to be repeated.
 */
template <typename SampleType>
class Shifter final
{
public:

	/** Creates a Shifter. The shifter will reference the specified Analyzer for the duration of its lifetime; make sure you don't delete the Analyzer before the Shifter!
	 */
	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	/** Destructor. */
	~Shifter();

	/** Sets the pitch, in Hz, of the shifter's output.
	    Note that before calling this, you must set the samplerate of the algorithm using Analyzer::setSamplerate()!
	 */
	void setPitch (int pitchHz) noexcept;

	/** Returns the next block of repitched samples from the shifter. */
	void getSamples (AudioBuffer<SampleType>& output);

	/** Returns the next block of repitched samples from the shifter. */
	void getSamples (SampleType* output, int numSamples);

	/** Skips a number of samples in the shifter's output (ie, for when it is bypassed). */
	void skipSamples (int numSamples);

	/** Returns the next repitched sample from the shifter. */
	[[nodiscard]] SampleType getNextSample();

	/** Resets the shifter to its initial state, without releasing any resources. Note that this is called for you if you call Analyzer::reset(). */
	void reset() noexcept;

	/** Releases all the resources used by the Shifter. Note that this is called for you if you call Analyzer::releaseResources(). */
	void releaseResources();

private:
	friend class Analyzer<SampleType>;

	void newBlockStarting() noexcept;

	void samplerateChanged() noexcept;

	void latencyChanged (int newLatency);

	/*-----------------------------------------------------------------------------------*/

	struct Grain final
	{
		using AnalysisGrain = typename Analyzer<SampleType>::Grain;

		~Grain();

		[[nodiscard]] SampleType getNextSample() noexcept;

		[[nodiscard]] bool isActive() const noexcept;

		void clearGrain() noexcept;

		void startNewGrain (AnalysisGrain& analysisGrainToUse) noexcept;

	private:
		AnalysisGrain* analysisGrain { nullptr };

		int sampleIdx { 0 };
	};

	[[nodiscard]] Grain& getGrainToStart();

	/*-----------------------------------------------------------------------------------*/

	Analyzer<SampleType>& analyzer;

	float targetPeriod { 0.f };

	int samplesToNextGrain { 0 }, placeInBlock { 0 }, targetPitchHz { 0 };

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
