
namespace bav
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}


SerializableData& SerializableData::operator= (SerializableData& other)
{
    deserialize (other.serialize());
    return *this;
}

ValueTree SerializableData::serialize()
{
    return serialize (dataIdentifier);
}

ValueTree SerializableData::serialize (juce::Identifier treeID)
{
    ValueTree tree {treeID};

    TreeReflector ref {tree, false};
    serialize (ref);

    return tree;
}


void SerializableData::deserialize (const ValueTree& t)
{
    if (t.hasType (dataIdentifier))
        setTree (t);
    else
        setTree (t.getChildWithName (dataIdentifier));
}

void SerializableData::setTree (const ValueTree& newTree)
{
    if (! newTree.isValid()) return;

    TreeReflector ref {newTree, true};
    serialize (ref);
}

}  // namespace bav
