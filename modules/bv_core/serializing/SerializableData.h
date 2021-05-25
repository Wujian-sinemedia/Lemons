
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
    void deserialize (const void* data, int sizeInBytes);
    
    
    const juce::Identifier dataIdentifier;
    
private:
    virtual void toValueTree (ValueTree& tree) = 0;
    virtual void fromValueTree (const ValueTree& tree) = 0;
};


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
