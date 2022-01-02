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

namespace lemons::dsp
{
/** @ingroup audio_engines
    Base class for audio engines that require latency (ie, a consistent block size) for their processing. The number of samples of the consistent blocksize the algorithm will recieve in its callbacks is the same as the algorithm's latecy in samples.

    @see Engine
 */
template <typename SampleType>
class LatencyEngine : public Engine<SampleType>
{
public:
	/** Returns the latency in samples of the engine. */
	[[nodiscard]] int reportLatency() const noexcept final;

	/** Sets the latency in samples of the engine.
	    Note that you must call Engine::prepare() at least once before calling this function, and the latency you specify here may be different than the blocksize you prepare the Engine base class with.
	    @param newInternalBlocksize The new latency (and internal blocksize) of the engine, in samples.
	    @param setTopLevelEngineBlocksize When true, this calls Engine::prepare() and sets the Engine base class's blocksize to be the new latency. This may be undesirable, because this will not allow you to call the top level process() function with blocksizes larger than the latency you've declared here.
	 */
	void changeLatency (int newInternalBlocksize, bool setTopLevelEngineBlocksize = false);

private:
	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void released() final;

	/** Your subclass should implement this function with your audio algorithm's rendering logic.
	    This function will always be called with blocks that are reportLatency() samples long.
	 */
	virtual void renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) = 0;

	/** Your subclass may implement this to be informed when the engine is released. */
	virtual void onRelease() { }

	/** Your subclass may implement this to be informed when the engine's latency is changed. */
	virtual void latencyChanged (int newLatency);


	int                          internalBlocksize { 0 };
	AudioAndMidiFIFO<SampleType> inputFIFO, outputFIFO;
	AudioBuffer<SampleType>      inBuffer, outBuffer;
	MidiBuffer                   chunkMidiBuffer;
};

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
struct LatencyEngineTests : public DspTest
{
public:
	LatencyEngineTests();

private:
	void runTest() final;

	struct PassThroughEngine : public dsp::LatencyEngine<FloatType>
	{
		void renderChunk (const AudioBuffer<FloatType>& input,
		                  AudioBuffer<FloatType>&       output,
		                  MidiBuffer& midiMessages, bool isBypassed) final;
	};

	PassThroughEngine engine;

	AudioBuffer<FloatType> audioIn, audioOut;

	MidiBuffer midiStorage;
};

LEMONS_CREATE_DSP_TEST (LatencyEngineTests)

}  // namespace lemons::tests

#endif
