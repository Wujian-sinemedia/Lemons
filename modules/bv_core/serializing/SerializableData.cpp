
namespace bav
{

SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{ }

ValueTree& SerializableData::serialize (ValueTree& tree)
{
    ValueTree child {dataIdentifier};
    toValueTree (child);
    tree.appendChild (child, nullptr);
    return tree;
}

void SerializableData::deserialize (const ValueTree& tree)
{
    auto t = tree.getChildWithName (dataIdentifier);
    if (t.isValid()) fromValueTree (t);
}
    

}  // namespace
