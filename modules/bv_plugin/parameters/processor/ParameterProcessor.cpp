
namespace bav
{

ParameterProcessorBase::ParameterProcessorBase (ParameterList& listToUse)
: list (listToUse)
{
    midiStorage.ensureSize (512);
}

template<typename SampleType>
void ParameterProcessorBase::process (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi)
{
    auto samplesLeft = audio.getNumSamples();
    auto midiIterator = midi.findNextSamplePosition (0);
    
    int startSample = 0;
    
    for (; samplesLeft > 0; ++midiIterator)
    {
        if (midiIterator == midi.cend())
        {
            processInternal (audio, midi, startSample, samplesLeft);
            return;
        }
        
        const auto metadata                 = *midiIterator;
        const auto samplesToNextMidiMessage = metadata.samplePosition - startSample;
        
        if (samplesToNextMidiMessage >= samplesLeft)
        {
            processInternal (audio, midi, startSample, samplesLeft);
            list.processMidiMessage (metadata.getMessage(), true);
            break;
        }
        
        if (samplesToNextMidiMessage == 0)
        {
            list.processMidiMessage (metadata.getMessage(), true);
            continue;
        }
        
        processInternal (audio, midi, startSample, samplesToNextMidiMessage);
        list.processMidiMessage (metadata.getMessage(), true);
        
        startSample += samplesToNextMidiMessage;
        samplesLeft -= samplesToNextMidiMessage;
    }
    
    std::for_each (
                   midiIterator, midi.cend(), [&] (const juce::MidiMessageMetadata& meta)
                   { list.processMidiMessage (meta.getMessage(), true); });
}

template void ParameterProcessorBase::process (juce::AudioBuffer<float>&, juce::MidiBuffer&);
template void ParameterProcessorBase::process (juce::AudioBuffer<double>&, juce::MidiBuffer&);


template<typename SampleType>
void ParameterProcessorBase::processInternal (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi,
                                          int startSample, int numSamples)
{
    juce::AudioBuffer<SampleType> alias {audio.getArrayOfWritePointers(),
                                         audio.getNumChannels(),
                                         startSample,
                                         numSamples};
    
    midi::copyRangeOfMidiBuffer (midi, midiStorage, startSample, 0, numSamples);
    
    renderChunk (alias, midiStorage);
    
    midi::copyRangeOfMidiBuffer (midiStorage, midi, 0, startSample, numSamples);
}

template void ParameterProcessorBase::processInternal (juce::AudioBuffer<float>&, juce::MidiBuffer&, int, int);
template void ParameterProcessorBase::processInternal (juce::AudioBuffer<double>&, juce::MidiBuffer&, int, int);

}
