
#pragma once

namespace bav
{
struct SerializableData
{
    SerializableData (juce::Identifier identifier);
    virtual ~SerializableData();
    
    bool operator== (const SerializableData& other) const;

    ValueTree  serialize (bool isPreset = false);
    ValueTree& serialize (ValueTree& tree, bool isPreset = false);

    void deserialize (const ValueTree& tree);
    
    const juce::Identifier& getDataIdentifier() const { return dataIdentifier; }
    
    void addDataChild (SerializableData& child, bool excludedFromPresets = false);
    void addDataChild (SerializableData* child, bool excludedFromPresets = false);

private:
    virtual void toValueTree (ValueTree& t)         = 0;
    virtual void fromValueTree (const ValueTree& t) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
    
    struct Child
    {
        Child (SerializableData& dataToUse, bool excludeFromPresets, SerializableData& parentData);
        ~Child();
        
        bool operator== (const Child& other) const;
        
        SerializableData& data;
        SerializableData& parent;
        bool              excludedFromPresets {false};
    };
    
    juce::Array<Child> children;
};


}  // namespace bav
