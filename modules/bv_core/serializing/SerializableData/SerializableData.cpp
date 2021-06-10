
namespace bav
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}


ValueTree SerializableData::serialize()
{
    toValueTree (tree);
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

void SerializableData::setTree (const ValueTree& newTree)
{
    tree = newTree;
    fromValueTree (newTree);
}


}  // namespace bav
