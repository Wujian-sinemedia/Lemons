
#pragma once

namespace bav
{

struct SerializableData
{
    using ValueTree = juce::ValueTree;
    
    SerializableData (juce::Identifier identifier);
    
    virtual ~SerializableData() = default;
    
    ValueTree& serialize (ValueTree& tree);
    
    void deserialize (const ValueTree& tree);
    
    
    const juce::Identifier dataIdentifier;
    
private:
    virtual void toValueTree (ValueTree& tree) const = 0;
    virtual void fromValueTree (const ValueTree& tree) = 0;
};


}  // namespace
