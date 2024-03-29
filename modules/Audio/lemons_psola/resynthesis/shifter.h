/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::dsp::psola
{

/** @defgroup lemons_psola_synthesis Resynthesis
	@ingroup lemons_psola
	PSOLA resynthesis classes.
 */

/** @ingroup lemons_psola_synthesis
	A class that repitches a stream of monophonic audio using PSOLA-like techniques.
	This class is essentially a "client" of an Analyzer object, which allows multiple Shifters to be used simultaneously without requiring the expensive analysis process to be repeated.
 */
template <typename SampleType>
class Shifter : public SampleStream<SampleType>
{
public:

	/** Creates a Shifter. The shifter will reference the specified Analyzer for the duration of its lifetime; make sure you don't delete the Analyzer before the Shifter!
	 */
	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	/** Destructor. */
	virtual ~Shifter();

	/** Sets the pitch, in Hz, of the shifter's output.
		Note that before calling this, you must set the samplerate of the algorithm using Analyzer::setSamplerate()!
	*/
	void setPitchHz (int pitchHz) noexcept;

	[[nodiscard]] float getPitchHz() const noexcept;

	/** Resets the shifter to its initial state, without releasing any resources. Note that this is called for you if you call Analyzer::reset(). */
	void reset() noexcept;

	/** Releases all the resources used by the Shifter. Note that this is called for you if you call Analyzer::releaseResources(). */
	void releaseResources();

protected:

	Analyzer<SampleType>& analyzer;

private:

	friend class Analyzer<SampleType>;

	[[nodiscard]] SampleType getNextSample();

	void newBlockStarting() noexcept;

	void samplerateChanged() noexcept;

	void latencyChanged (int newLatency);

	virtual void onNewBlock() { }
	virtual void pitchHzChanged (int /*pitchHz*/) { }

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

	float targetPeriod { 0.f };

	int samplesToNextGrain { 0 }, placeInBlock { 0 }, targetPitchHz { 0 };

	ConstructedArray<Grain> grains;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Shifter)
};

}  // namespace lemons::dsp::psola
