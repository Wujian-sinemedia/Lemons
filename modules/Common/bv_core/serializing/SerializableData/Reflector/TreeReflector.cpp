
namespace bav
{
TreeReflector::TreeReflector (ValueTree treeToUse, bool loading)
    : tree (treeToUse), loadingData (loading)
{
}

bool TreeReflector::isLoading() const { return loadingData; }

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.saveToTree (propertyName), nullptr);
}

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data) const
{
    data.restoreFromTree (tree.getChildWithName (propertyName));
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }

}  // namespace bav
