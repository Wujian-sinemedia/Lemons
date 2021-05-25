
namespace bav
{

SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{ }


juce::ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};
    toValueTree (tree);
    return tree;
}


juce::ValueTree& SerializableData::serialize (ValueTree& tree)
{
    ValueTree child {dataIdentifier};
    toValueTree (child);
    tree.appendChild (child, nullptr);
    return tree;
}


void SerializableData::deserialize (const ValueTree& tree)
{
    auto t = tree.getChildWithName (dataIdentifier);
    jassert (t.isValid());
    fromValueTree (t);
}
    

}  // namespace
