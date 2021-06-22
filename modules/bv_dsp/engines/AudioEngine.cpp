
namespace bav::dsp
{
template < typename SampleType >
void Engine< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0 && blocksize > 0);
    dummyMidiBuffer.ensureSize (static_cast< size_t > (blocksize));
    hasBeenInitialized = true;
    sampleRate         = samplerate;
    prepared (blocksize, samplerate);
}

template < typename SampleType >
void Engine< SampleType >::releaseResources()
{
    dummyMidiBuffer.clear();
    hasBeenInitialized      = false;
    wasBypassedLastCallback = true;
    sampleRate              = 0.;
    released();
}

template < typename SampleType >
void Engine< SampleType >::process (AudioBuffer& inplaceInAndOut,
                                    MidiBuffer&  midiMessages,
                                    bool         isBypassed)
{
    processInternal (inplaceInAndOut, inplaceInAndOut, midiMessages, isBypassed);
}

template < typename SampleType >
void Engine< SampleType >::process (const AudioBuffer& input,
                                    AudioBuffer&       output,
                                    MidiBuffer&        midiMessages,
                                    bool               isBypassed)
{
    processInternal (input, output, midiMessages, isBypassed);
}

template < typename SampleType >
void Engine< SampleType >::process (AudioBuffer& inplaceInAndOut,
                                    bool         isBypassed)
{
    dummyMidiBuffer.clear();
    processInternal (inplaceInAndOut, inplaceInAndOut, dummyMidiBuffer, isBypassed);
}

template < typename SampleType >
void Engine< SampleType >::process (const AudioBuffer& input,
                                    AudioBuffer&       output,
                                    bool               isBypassed)
{
    dummyMidiBuffer.clear();
    processInternal (input, output, dummyMidiBuffer, isBypassed);
}

template < typename SampleType >
void Engine< SampleType >::processInternal (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    jassert (isInitialized() && sampleRate > 0);

    bool processingBypassedThisFrame, applyFadeIn, applyFadeOut;

    if (isBypassed)
    {
        processingBypassedThisFrame = wasBypassedLastCallback;
        applyFadeIn                 = false;
        applyFadeOut                = ! wasBypassedLastCallback;
    }
    else
    {
        processingBypassedThisFrame = false;
        applyFadeIn                 = wasBypassedLastCallback;
        applyFadeOut                = false;
    }

    wasBypassedLastCallback = isBypassed;

    renderBlock (input, output, midiMessages, processingBypassedThisFrame);

    const auto numSamples = output.getNumSamples();

    if (applyFadeIn)
        output.applyGainRamp (0, numSamples, SampleType (0.), SampleType (1.));

    if (applyFadeOut)
        output.applyGainRamp (0, numSamples, SampleType (1.), SampleType (0.));
}

template < typename SampleType >
void Engine< SampleType >::prepared (int, double)
{
}


template class Engine< float >;
template class Engine< double >;

}  // namespace bav::dsp
