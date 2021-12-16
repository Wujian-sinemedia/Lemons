#include <lemons_core/lemons_core.h>

namespace lemons::serializing
{

MidiBuffer midiBufferFromFile (const juce::MidiFile& file, int trackToRead)
{
    MidiBuffer buffer;
    
    const auto addEventsFromTrack = [&] (int trackNum)
    {
        if (const auto* track = file.getTrack (trackNum))
        {
            for (const auto* holder : *track)
                buffer.addEvent (holder->message,
                                 juce::roundToInt (holder->message.getTimeStamp()));
        }
    };
    
    if (trackToRead > -1)
    {
        addEventsFromTrack (trackToRead);
    }
    else
    {
        for (int i = 0; i < file.getNumTracks(); ++i)
            addEventsFromTrack (i);
    }
    
    jassert (juce::roundToInt (file.getLastTimestamp()) == buffer.getLastEventTime());
    
    return buffer;
}

juce::MidiFile midiBufferToFile (const MidiBuffer& midi)
{
    juce::MidiMessageSequence seq;
    
    for (const auto& meta : midi)
        seq.addEvent (meta.getMessage());
    
    jassert (seq.getNumEvents() == midi.getNumEvents());
    
    juce::MidiFile file;
    
    file.addTrack (seq);
    
    jassert (file.getLastTimestamp() == midi.getLastEventTime());
    
    return file;
}

MidiBuffer midiFromBinary (const MemoryBlock& block)
{
    juce::MemoryInputStream stream { block, false };
    
    juce::MidiFile file;
    file.readFrom (stream);
    
    return midiBufferFromFile (file);
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
    const auto file = midiBufferToFile (midi);
    
    MemoryBlock              block;
    juce::MemoryOutputStream stream { block, false };
    
    file.writeTo (stream);
    
    return block;
}

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

using namespace lemons::serializing;

MidiBuffer VariantConverter<MidiBuffer>::fromVar (const var& v)
{
    return midiFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<MidiBuffer>::toVar (const MidiBuffer& b)
{
    return { memoryBlockToString (midiToBinary (b)) };
}

MidiFile VariantConverter<MidiFile>::fromVar (const var& v)
{
    juce::MidiFile file;
    
    if (const auto* block = v.getBinaryData())
    {
        juce::MemoryInputStream stream { *block, false };
        file.readFrom (stream);
    }
    
    return file;
}

var VariantConverter<MidiFile>::toVar (const MidiFile& f)
{
    MemoryBlock              block;
    juce::MemoryOutputStream stream { block, false };
    
    f.writeTo (stream);
    
    return { block };
}

MidiMessage VariantConverter<MidiMessage>::fromVar (const var& v)
{
    if (const auto* obj = v.getDynamicObject())
    {
        if (obj->hasProperty (data_prop))
        {
            if (const auto* data = obj->getProperty (data_prop).getBinaryData())
            {
                const auto timestamp = [&]() -> double
                {
                    if (obj->hasProperty (time_prop))
                        return obj->getProperty (time_prop);
                    
                    return 0.;
                }();
                
                return { data->getData(), static_cast<int> (data->getSize()), timestamp };
            }
        }
    }
    
    return {};
}

var VariantConverter<MidiMessage>::toVar (const MidiMessage& m)
{
    DynamicObject obj;
    
    MemoryBlock block { m.getRawData(), static_cast<size_t> (m.getRawDataSize()) };
    
    obj.setProperty (data_prop, block);
    obj.setProperty (time_prop, m.getTimeStamp());
    
    return { obj.clone().get() };
}

}
