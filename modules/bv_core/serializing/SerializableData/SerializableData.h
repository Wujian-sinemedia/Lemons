
#pragma once

namespace bav
{
struct SerializableData
{
    SerializableData (juce::Identifier identifier);
    virtual ~SerializableData();

    bool operator== (const SerializableData& other) const;

    ValueTree  serialize();
    ValueTree& serialize (ValueTree& tree);

    void deserialize (const ValueTree& tree);

    const juce::Identifier& getDataIdentifier() const { return dataIdentifier; }

    void addDataChild (SerializableData& child);
    void addDataChild (SerializableData* child);

private:
    struct Child;
    
    virtual void toValueTree (ValueTree& t)         = 0;
    virtual void fromValueTree (const ValueTree& t) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
    
    juce::Array< Child > children;
};


}  // namespace bav
