
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
        data = tree;
    else
        tree = data;
}

void TreeReflector::as (SerializableData& data)
{
    data.serialize (*this);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }


/*------------------------------------------------------------------------------------------------------*/

bool TreeLoader::isLoading() const { return true; }

bool TreeSaver::isLoading() const { return false; }

}  // namespace lemons
