
namespace bav
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}


juce::ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};
    toValueTree (tree);
    return tree;
}


juce::ValueTree& SerializableData::serialize (ValueTree& tree)
{
    tree.appendChild (serialize(), nullptr);
    return tree;
}


void SerializableData::deserialize (const ValueTree& tree)
{
    if (tree.hasType (dataIdentifier))
    {
        fromValueTree (tree);
    }
    else
    {
        fromValueTree (tree.getChildWithName (dataIdentifier));
    }
}


}  // namespace bav
