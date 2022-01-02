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
/** Base class for an audio engine.
    This should be a self-contained signal chain. This class handles pop-preventing between succesive bypassed/unbypassed frames, and provides several options for top-level process() function APIs while allowing you to implement only one internal renderBlock() function.
    @see LatencyEngine
 */
template <typename SampleType>
class Engine
{
public:
	/** Creates a default audio engine. */
	explicit Engine() = default;

	/** Destructor. */
	virtual ~Engine() = default;

    /** @name Processing */
    ///@{

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

    ///@}
    

	/** Returns true if prepare() has been called at least once since the object's construction or the last releaseResources() call.
	    @see prepare(), releaseResources()
	 */
	[[nodiscard]] bool isInitialized() const noexcept;


	/** Prepares the engine. */
	void prepare (double samplerate, int blocksize, int numChannels = 2);

	/** Releases all of the engine's internal resources. */
	void releaseResources();

	/** Returns the latency in samples of this engine. */
	[[nodiscard]] virtual int reportLatency() const noexcept;

	/** Returns the engine's samplerate.
	    This will return 0. if the engine is unprepared.
	 */
	[[nodiscard]] double getSamplerate() const noexcept;

	/** Returns the number of channels this engine has been prepared to process.
	    This will return 0 if the engine is unprepared.
	 */
	[[nodiscard]] int getNumChannels() const noexcept;

	/** Returns the maximum blocksize this engine has been prepared to process.
	    This will return 0 if the engine is unprepared.
	 */
	[[nodiscard]] int getBlocksize() const noexcept;

	/** Call this method to reset the engine, without deallocating resources -- for example, stopping all voices of a synth.
	 */
	void reset();

	/** The engine's sample type is publicly accessible via this typedef. */
	using ValueType = SampleType;

private:
	void processInternal (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed);

	/** Your subclass should implement this with your audio algorithm's rendering logic. */
	virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) = 0;

	/** Your subclass may implement this to be informed when the engine is prepared. */
	virtual void prepared (int blocksize, double samplerate, int numChannels);

	/** Your subclass may implement this to be informed when the engine is released. */
	virtual void released() { }

	/** Your subclass may implement this to be informed when reset() is called.
	    @attention You should NOT deallocate any resources in here, as process() may be called again before another call to prepare()!
	 */
	virtual void onReset() { }

	MidiBuffer dummyMidiBuffer;
	bool       wasBypassedLastCallback { true };
	double     sampleRate { 0. };
	int        blockSize { 0 };

	AudioBuffer<SampleType> outputStorage;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Engine)
};


#if LEMONS_UNIT_TESTS

template <typename SampleType>
class PassThroughEngine : public Engine<SampleType>
{
	void renderBlock (const AudioBuffer<SampleType>& input,
	                  AudioBuffer<SampleType>&       output,
	                  MidiBuffer& midiMessages, bool isBypassed) final;
};

#endif

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

	dsp::PassThroughEngine<FloatType> engine;

	AudioBuffer<FloatType> audioIn, audioOut;

	MidiBuffer midiStorage;
};

LEMONS_CREATE_DSP_TEST (AudioEngineTests)

}  // namespace lemons::tests

#endif
