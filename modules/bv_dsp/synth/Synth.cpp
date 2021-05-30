
#include "internals/PanningManager/PanningManager.cpp"
#include "internals/AutomatedHarmonyVoice.cpp"

namespace bav::dsp
{
/*
 Constructor.
 */
template < typename SampleType >
SynthBase< SampleType >::SynthBase()
    : bendTracker (2, 2)
      , lastBlocksize (0)
{
    adsrParams.attack  = 0.035f;
    adsrParams.decay   = 0.06f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.01f;

    quickReleaseParams.attack  = 0.01f;
    quickReleaseParams.decay   = 0.005f;
    quickReleaseParams.sustain = 1.0f;
    quickReleaseParams.release = 0.015f;

    setCurrentPlaybackSampleRate (44100.0);

    setConcertPitchHz (440);
}


/*
 Destructor.
 */
template < typename SampleType >
SynthBase< SampleType >::~SynthBase()
{
    voices.clear();
}


/*
 Initializes the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize)
{
    jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);

    changeNumVoices (initNumVoices);

    setCurrentPlaybackSampleRate (initSamplerate);

    initialized (initSamplerate, initBlocksize);

    prepare (initBlocksize);
}


/*
 Immediately clears all notes & resets the synth's internal state.
 */
template < typename SampleType >
void SynthBase< SampleType >::reset()
{
    for (auto* voice : voices)
        voice->clearCurrentNote();

    panner.reset();

    resetTriggered();
}


/*
 Prepares the synth for a new expected maximum blocksize.
 */
template < typename SampleType >
void SynthBase< SampleType >::prepare (const int blocksize)
{
    jassert (blocksize > 0);
    jassert (! voices.isEmpty());

    aggregateMidiBuffer.ensureSize (size_t (blocksize * 2));
    aggregateMidiBuffer.clear();

    for (auto* voice : voices)
        voice->prepare (blocksize);

    panner.prepare (voices.size(), false);
    
    resetRampedValues (blocksize);

    prepared (blocksize);
}


/*
 Attempts to reduce the synth's footprint as much as possible.
 After calling this, you must call initialize() again before attempting to render any more audio with the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::releaseResources()
{
    aggregateMidiBuffer.clear();
    usableVoices.clear();
    currentNotes.clear();
    desiredNotes.clear();

    panner.releaseResources();

    for (auto* voice : voices)
        voice->release();

    release();
}


/*=========================================================================================================
          _    _ _____ _____ ____     _____  ______ _   _ _____  ______ _____  _____ _   _  _____
     /\  | |  | |  __ \_   _/ __ \   |  __ \|  ____| \ | |  __ \|  ____|  __ \|_   _| \ | |/ ____|
    /  \ | |  | | |  | || || |  | |  | |__) | |__  |  \| | |  | | |__  | |__) | | | |  \| | |  __
   / /\ \| |  | | |  | || || |  | |  |  _  /|  __| | . ` | |  | |  __| |  _  /  | | | . ` | | |_ |
  / ____ \ |__| | |__| || || |__| |  | | \ \| |____| |\  | |__| | |____| | \ \ _| |_| |\  | |__| |
 /_/    \_\____/|_____/_____\____/   |_|  \_\______|_| \_|_____/|______|_|  \_\_____|_| \_|\_____|
 
 Functions for audio rendering
 
 ==========================================================================================================*/

/*
 Renders the synth's composite stereo output to "output". (If a mono buffer is passed, the synth will return mono output.)
 Reads events from and returns the synth's composite midi output to "midiMessages". Note that due to the midi latch, pedal pitch, and descant features, the returned midi buffer may have more or fewer events than the original passed midi buffer.
 Internally, the synth will render smaller chunks of audio in between each midi message timestamp, to keep midi events synchronous with the audio output. If you are implementing a class that derives from this class or the SynthVoiceBase class, note that the SynthVoiceBase's renderPlease() method may recieve blocksizes as small as 1 sample (but never 0 samples).
 */
template < typename SampleType >
void SynthBase< SampleType >::renderVoices (juce::MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output)
{
    jassert (! voices.isEmpty());
    jassert (sampleRate > 0);

    output.clear();

    aggregateMidiBuffer.clear();
    
    auto samplesLeft = output.getNumSamples();

    midiInputStorage.clear();
    midiInputStorage.addEvents (midiMessages, 0, samplesLeft, 0);

    for (auto* voice : voices)
        voice->newBlockComing (lastBlocksize, samplesLeft);

    lastBlocksize = samplesLeft;

    auto midiIterator = midiMessages.findNextSamplePosition (0);

    int startSample = 0;

    for (; samplesLeft > 0; ++midiIterator)
    {
        if (midiIterator == midiMessages.cend())
        {
            renderVoicesInternal (output, startSample, samplesLeft);
            midiMessages.swapWith (aggregateMidiBuffer);
            midiInputStorage.clear();
            return;
        }

        const auto metadata                 = *midiIterator;
        const auto samplesToNextMidiMessage = metadata.samplePosition - startSample;

        if (samplesToNextMidiMessage >= samplesLeft)
        {
            renderVoicesInternal (output, startSample, samplesLeft);
            midi.process (metadata);
            break;
        }

        if (samplesToNextMidiMessage == 0)
        {
            midi.process (metadata);
            continue;
        }

        renderVoicesInternal (output, startSample, samplesToNextMidiMessage);
        midi.process (metadata);

        startSample += samplesToNextMidiMessage;
        samplesLeft -= samplesToNextMidiMessage;
    }

    std::for_each (
        midiIterator, midiMessages.cend(), [&] (const juce::MidiMessageMetadata& meta)
                   { midi.process (meta); });

    midiMessages.swapWith (aggregateMidiBuffer);
    midiInputStorage.clear();
}


/*
 Renders all the synth's voices for the given range of the output buffer. (Not for public use.)
 */
template < typename SampleType >
void SynthBase< SampleType >::renderVoicesInternal (juce::AudioBuffer< SampleType >& output, const int startSample, const int numSamples)
{
#if JUCE_DEBUG
    const auto totalNumSamples = output.getNumSamples();
    jassert (numSamples <= totalNumSamples && startSample < totalNumSamples);
#endif
    for (auto* voice : voices)
    {
        if (voice->isVoiceActive())
            voice->renderBlock (output, startSample, numSamples);
        else
            voice->bypassedBlock (numSamples);
    }
}


/*
 This should be called when a block is recieved while the processor is in bypassed mode.
 This will not output any audio, but may alter the contents of the MidiBuffer.
 */
template < typename SampleType >
void SynthBase< SampleType >::bypassedBlock (const int numSamples, MidiBuffer& midiMessages)
{
    midi.process (midiMessages);

    for (auto* voice : voices)
        voice->bypassedBlock (numSamples);
}


/*
 Sets the synth's current playback samplerate.
 This MUST be called before you attempt to render any audio with the synth!
 */
template < typename SampleType >
void SynthBase< SampleType >::setCurrentPlaybackSampleRate (const double newRate)
{
    jassert (newRate > 0);

    sampleRate = newRate;

    for (auto* voice : voices)
        voice->updateSampleRate (newRate);

    samplerateChanged (newRate);
}


/*=========================================================================================================
  __  __ _____  _____  _____      ______ _    _ _   _  _____ _______ _____ ____  _   _  _____
 |  \/  |_   _|/ ____|/ ____|    |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
 | \  / | | | | (___ | |         | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___
 | |\/| | | |  \___ \| |         |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \
 | |  | |_| |_ ____) | |____ _   | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
 |_|  |_|_____|_____/ \_____(_)  |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/
 
 Functions that don't really belong anywhere else
 
 ==========================================================================================================*/

template < typename SampleType >
float SynthBase< SampleType >::getOutputFrequency (const int midipitch, const int midiChannel) const
{
    return pitchConverter.midiToFrequency (bendTracker.newNoteRecieved(midipitch), midiChannel);
}


/*
 Resets the voices' ramped gain values, and prepares them for a new blocksize.
 This should be called with/by prepare().
 */
template < typename SampleType >
void SynthBase< SampleType >::resetRampedValues (int blocksize)
{
    for (auto* voice : voices)
        voice->resetRampedValues (blocksize);
}


template class SynthBase< float >;
template class SynthBase< double >;


}  // namespace bav::dsp
