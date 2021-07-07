
namespace bav
{
TreeReflector::TreeReflector (ValueTree treeToUse, bool loading)
    : tree (treeToUse), loadingData (loading)
{
}

bool TreeReflector::isLoading() const { return loadingData; }

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.serialize (juce::Identifier (propertyName)), nullptr);
}

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data) const
{
    const auto child = tree.getChildWithName (juce::Identifier (propertyName));

    if (child.isValid())
        data.deserialize (child);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }

}  // namespace bav
