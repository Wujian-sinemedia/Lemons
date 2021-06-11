
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
    virtual void toValueTree (ValueTree& t)         = 0;
    virtual void fromValueTree (const ValueTree& t) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;

    struct Child
    {
        Child (SerializableData& dataToUse, SerializableData& parentData);
        ~Child();

        bool operator== (const Child& other) const;

        SerializableData& data;
        SerializableData& parent;
    };

    juce::Array< Child > children;
};


}  // namespace bav
