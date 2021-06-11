
namespace bav
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}

SerializableData::~SerializableData()
{
}


bool SerializableData::operator== (const SerializableData& other) const
{
    return dataIdentifier == other.dataIdentifier;
}


ValueTree SerializableData::serialize (bool isPreset)
{
    ValueTree tree {dataIdentifier};
    
    toValueTree (tree);
    
    for (auto& child : children)
        if (! isPreset || ! child.excludedFromPresets)
            child.data.serialize (tree, isPreset);
    
    return tree;
}


ValueTree& SerializableData::serialize (ValueTree& t, bool isPreset)
{
    t.appendChild (serialize (isPreset), nullptr);
    return t;
}


void SerializableData::deserialize (const ValueTree& t)
{
    if (t.hasType (dataIdentifier))
    {
        setTree (t);
    }
    else
    {
        setTree (t.getChildWithName (dataIdentifier));
    }
}

void SerializableData::addDataChild (SerializableData& child, bool excludedFromPresets)
{
    jassert (&child != this);
    children.addIfNotAlreadyThere ({ child, excludedFromPresets, *this });
}

void SerializableData::addDataChild (SerializableData* child, bool excludedFromPresets)
{
    if (child != nullptr)
        addDataChild (*child, excludedFromPresets);
}

void SerializableData::setTree (const ValueTree& newTree)
{
    if (! newTree.isValid()) return;
    
    fromValueTree (newTree);
    
    for (auto& child : children)
        child.data.deserialize (newTree);
}

SerializableData::Child::Child (SerializableData& dataToUse, bool excludeFromPresets, SerializableData& parentData)
: data(dataToUse), parent(parentData), excludedFromPresets(excludeFromPresets)
{
}

SerializableData::Child::~Child()
{
    parent.children.removeIf ([=](const Child& child){ return child == *this; });
}

bool SerializableData::Child::operator== (const Child& other) const
{
    return &data == &other.data;
}


}  // namespace bav
