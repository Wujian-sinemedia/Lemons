#pragma once

namespace lemons::dsp
{
/** Base class for audio engines that require latency (ie, a consistent block size) for their processing.

    @see Engine
 */
template <typename SampleType>
class LatencyEngine : public Engine<SampleType>
{
public:
	/** Returns the latency in samples of the engine. */
	int reportLatency() const final { return internalBlocksize; }

	/** Sets the samplerate of the engine.
	    This function exists so that you can update only the samplerate, without needing to call Engine::prepare() and provide the blocksize again.
	 */
	void setSamplerate (double samplerate);
    
protected:
    /** Sets the latency in samples of the engine. */
    void changeLatency (int newInternalBlocksize);

private:
	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void prepared (int blocksize, double samplerate, int numChannels) final;
	void released() final;

	/** Your subclass should implement this function with your audio algorithm's rendering logic.
	    This function will always be called with blocks that are reportLatency() samples long.
	 */
	virtual void renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) = 0;

	/** Your subclass may implement this to be informed when the engine is prepared. */
	virtual void onPrepare (int blocksize, double samplerate, int numChannels);

	/** Your subclass may implement this to be informed when the engine is released. */
	virtual void onRelease() { }


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

	void testLatency (int latency);

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
