
#pragma once

namespace bav
{

struct SerializableData
{
    using ValueTree = juce::ValueTree;
    
    SerializableData (juce::Identifier identifier);
    virtual ~SerializableData() = default;
    
    ValueTree serialize();
    ValueTree& serialize (ValueTree& tree);
    
    void deserialize (const ValueTree& tree);
    
    const juce::Identifier dataIdentifier;
    
private:
    virtual void toValueTree (ValueTree& tree) = 0;
    virtual void fromValueTree (const ValueTree& tree) = 0;
};


/*-------------------------------------------------------------*/


extern void toBinary (SerializableData& data, juce::File& file);
extern void toBinary (SerializableData& data, juce::MemoryBlock& dest);

extern void fromBinary (juce::File& file, SerializableData& dest);
extern void fromBinary (juce::MemoryBlock& data, SerializableData& dest);
extern void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest);
extern void fromBinary (const void* data, int dataSizeInBytes, SerializableData& dest);


/*-------------------------------------------------------------*/


class DataSynchronizer :   private juce::Timer
{
public:
    explicit DataSynchronizer (SerializableData& dataToUse);
    virtual ~DataSynchronizer() override;
    
    void applyChangeData (const void* data, size_t dataSize);
    
private:
    void timerCallback() override final;
    
    virtual void sendChangeData (const void* data, size_t dataSize) const = 0;
    
    SerializableData& sData;
};


}  // namespace
