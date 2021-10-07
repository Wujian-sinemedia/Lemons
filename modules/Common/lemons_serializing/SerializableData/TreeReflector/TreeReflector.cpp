
namespace lemons
{
TreeReflector::TreeReflector (const ValueTree& treeToUse)
    : tree (treeToUse)
{
    jassert (tree.isValid());
}

bool TreeReflector::isSaving() const { return ! isLoading(); }

void TreeReflector::addValueTree (ValueTree& data)
{
    if (isLoading())
        tree.copyPropertiesFrom (data, nullptr);
    else
        data.copyPropertiesFrom (tree, nullptr);
}

void TreeReflector::addSerializableData (SerializableData& data)
{
    if (isLoading())
        data.restoreFromTree (tree);
    else
        data.saveToTree (tree);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }


/*------------------------------------------------------------------------------------------------------*/

bool TreeLoader::isLoading() const { return true; }

bool TreeSaver::isLoading() const { return false; }

}  // namespace lemons
