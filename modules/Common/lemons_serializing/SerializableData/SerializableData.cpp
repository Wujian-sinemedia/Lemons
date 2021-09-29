
namespace lemons
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
    return saveToTree (dataIdentifier.toString());
}

void SerializableData::deserialize (const ValueTree& t)
{
    if (t.hasType (dataIdentifier))
        restoreFromTree (t);
    else
        restoreFromTree (t.getChildWithName (dataIdentifier));
}

ValueTree SerializableData::saveToTree (const String& treeName)
{
    ValueTree tree {treeName};
    TreeSaver ref {tree};

    serialize (ref);
    return tree;
}

void SerializableData::restoreFromTree (const ValueTree& newTree)
{
    if (! newTree.isValid()) return;

    TreeLoader ref {newTree};

    serialize (ref);
}

}  // namespace bav
