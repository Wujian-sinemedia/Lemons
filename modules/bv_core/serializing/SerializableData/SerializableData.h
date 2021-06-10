
#pragma once

namespace bav
{
struct SerializableData
{
    SerializableData (juce::Identifier identifier);
    virtual ~SerializableData() = default;

    ValueTree  serialize();
    ValueTree& serialize (ValueTree& tree);

    void deserialize (const ValueTree& tree);
    
    juce::Identifier getDataIdentifier() const { return dataIdentifier; }

private:
    virtual void toValueTree (ValueTree& t)         = 0;
    virtual void fromValueTree (const ValueTree& t) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
    ValueTree tree {dataIdentifier};
};


}  // namespace bav
