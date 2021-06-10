
namespace bav
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}

SerializableData::~SerializableData()
{
    for (auto& pntr : children)
        pntr->parent = nullptr;
    
    if (parent != nullptr)
        parent->children.removeAllInstancesOf (this);
}


bool SerializableData::operator== (const SerializableData& other) const
{
    return dataIdentifier == other.dataIdentifier;
}


ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};
    
    toValueTree (tree);
    
    for (auto& pntr : children)
        pntr->serialize (tree);
    
    return tree;
}


ValueTree& SerializableData::serialize (ValueTree& t)
{
    t.appendChild (serialize(), nullptr);
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

void SerializableData::addDataChild (SerializableData& child)
{
    jassert (&child != this);
    
    if (children.addIfNotAlreadyThere (&child))
        child.parent = this;
}

void SerializableData::addDataChild (SerializableData* child)
{
    if (child != nullptr)
        addDataChild (*child);
}

void SerializableData::setTree (const ValueTree& newTree)
{
    fromValueTree (newTree);
    
    for (auto& pntr : children)
        pntr->deserialize (newTree);
}


}  // namespace bav
