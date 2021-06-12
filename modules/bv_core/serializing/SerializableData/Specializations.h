
#pragma once

namespace bav
{

struct SerializableFile : SerializableData
{
    SerializableFile (juce::File fileToUse, juce::Identifier dataID);
    
private:
    void toValueTree (ValueTree& t) final;
    void fromValueTree (const ValueTree& t) final;
    
    juce::File file;
    const juce::Identifier fileContentsPropertyName;
};

/*----------------------------------------------------------------------------*/

template<typename SampleType>
struct AudioBufferSerializer : SerializableData
{
public:
    AudioBufferSerializer (juce::AudioBuffer<SampleType>& bufferToSerialize, juce::Identifier dataID);
    
private:
    void toValueTree (ValueTree& t) final;
    void fromValueTree (const ValueTree& t) final;
    
    juce::AudioBuffer<SampleType>& buffer;
};

}
