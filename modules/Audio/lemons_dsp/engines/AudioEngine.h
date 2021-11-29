#pragma once

namespace lemons::dsp
{
/** Base class for an audio engine.
    This should be a self-contained signal chain. This class handles pop-preventing between succesive bypassed/unbypassed frames, and provides several options for top-level process() function APIs while allowing you to implement only one internal renderBlock() function.
    @see LatencyEngine
 */
template <typename SampleType>
class Engine
{
public:
	/** Creates a default audio engine. */
	Engine() = default;

	/** Destructor. */
	virtual ~Engine() = default;

	/** Returns the latency in samples of this engine. */
	virtual int reportLatency() const { return 0; }


	/** Processes the audio engine with in-place audio I/O and MIDI.
	    @param inplaceInAndOut The audio buffer that the input will be read from and the output will be written to.
	    @param midiMessages MIDI I/O buffer. The engine's MIDI output will be returned in-place.
	    @param isBypassed Should be true if the engine is bypassed this frame.
	 */
	void process (AudioBuffer<SampleType>& inplaceInAndOut,
	              MidiBuffer&              midiMessages,
	              bool                     isBypassed = false);


	/** Processes the audio engine with separate audio in and out, and MIDI.
	    @param input The audio input.
	    @param output The audio output.
	    @param midiMessages MIDI I/O buffer. The engine's MIDI output will be returned in-place.
	    @param isBypassed Should be true if the engine is bypassed this frame.
	 */
	void process (const AudioBuffer<SampleType>& input,
	              AudioBuffer<SampleType>&       output,
	              MidiBuffer&                    midiMessages,
	              bool                           isBypassed = false);


	/** Processes the audio engine with in-place audio I/O and no MIDI.
	    The internal renderBlock() function will be called with an empty dummy MidiBuffer.
	    @param inplaceInAndOut The audio buffer that the input will be read from and the output will be written to.
	    @param isBypassed Should be true if the engine is bypassed this frame.
	 */
	void process (AudioBuffer<SampleType>& inplaceInAndOut,
	              bool                     isBypassed = false);


	/** Processes the audio engine with separate audio in and out and no MIDI.
	    The internal renderBlock() function will be called with an empty dummy MidiBuffer.
	    @param input The audio input.
	    @param output The audio output.
	    @param isBypassed Should be true if the engine is bypassed this frame.
	 */
	void process (const AudioBuffer<SampleType>& input,
	              AudioBuffer<SampleType>&       output,
	              bool                           isBypassed = false);


	/** Returns true if prepare() has been called at least once since the object's construction or the last releaseResources() call.
	    @see prepare(), releaseResources()
	 */
	bool isInitialized() const noexcept { return sampleRate > 0.; }


	/** Prepares the engine. */
	void prepare (double samplerate, int blocksize, int numChannels = 2);

	/** Releases all of the engine's internal resources. */
	void releaseResources();

	/** Returns the engine's samplerate. */
	double getSamplerate() const noexcept { return sampleRate; }

private:
	void processInternal (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed);

	/** Your subclass should implement this with your audio algorithm's rendering logic. */
	virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) = 0;

	/** Your subclass may implement this to be informed when the engine is prepared. */
	virtual void prepared (int blocksize, double samplerate, int numChannels);

	/** Your subclass may implement this to be informed when the engine is released. */
	virtual void released() { }

	MidiBuffer dummyMidiBuffer;
	bool       wasBypassedLastCallback { true };
	double     sampleRate { 0. };

	AudioBuffer<SampleType> outputStorage;
};

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
struct AudioEngineTests : public DspTest
{
public:
    AudioEngineTests();
    
private:
    void runTest() final;
    
    struct PassThroughEngine : public dsp::Engine<FloatType>
    {
        void renderBlock (const AudioBuffer<FloatType>& input,
                          AudioBuffer<FloatType>& output,
                          MidiBuffer& midiMessages, bool isBypassed) final;
    };
    
    PassThroughEngine engine;
    
    AudioBuffer<FloatType> audioIn, audioOut;
    
    MidiBuffer midiStorage;
    
    dsp::osc::Sine<FloatType> osc;
};

static AudioEngineTests<float>  audioEngineTest_float;
static AudioEngineTests<double> audioEngineTest_double;

}  // namespace lemons::tests

#endif
