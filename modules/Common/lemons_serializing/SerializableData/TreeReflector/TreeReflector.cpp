
namespace lemons
{
TreeReflector::TreeReflector (const ValueTree& treeToUse)
    : tree (treeToUse)
{
	jassert (tree.isValid());
}

[[nodiscard]] bool TreeReflector::isSaving() const { return ! isLoading(); }

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

[[nodiscard]] ValueTree& TreeReflector::getRawDataTree() { return tree; }


/*------------------------------------------------------------------------------------------------------*/

[[nodiscard]] bool TreeLoader::isLoading() const { return true; }

[[nodiscard]] bool TreeSaver::isLoading() const { return false; }

}  // namespace lemons
