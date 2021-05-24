
#pragma once

namespace bav
{

struct SerializableData
{
    using ValueTree = juce::ValueTree;
    
    SerializableData (juce::Identifier identifier)
        : dataIdentifier (identifier)
    { }
    
    virtual ~SerializableData() = default;
    
    void serialize (ValueTree& tree)
    {
        ValueTree child {dataIdentifier};
        toValueTree (child);
        tree.appendChild (child, nullptr);
    }
    
    void deserialize (const ValueTree& tree)
    {
        auto t = tree.getChildWithName (dataIdentifier);
        if (t.isValid()) fromValueTree (t);
    }
    
    const juce::Identifier dataIdentifier;
    
private:
    virtual void toValueTree (ValueTree& tree) const = 0;
    virtual void fromValueTree (const ValueTree& tree) = 0;
};


}  // namespace
