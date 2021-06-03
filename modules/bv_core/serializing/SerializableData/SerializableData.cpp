
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
    jassert (tree.isValid());

    if (tree.hasType (dataIdentifier))
    {
        fromValueTree (tree);
        return;
    }

    auto t = tree.getChildWithName (dataIdentifier);
    jassert (t.isValid());
    fromValueTree (t);
}


}  // namespace bav
