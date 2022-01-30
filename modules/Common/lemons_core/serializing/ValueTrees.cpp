
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

namespace juce
{

ValueTree VariantConverter<ValueTree>::fromVar (const var& obj)
{
	if (auto* dobj = obj.getDynamicObject(); dobj->hasProperty (NAME_PROP))
	{
		ValueTree tree { dobj->getProperty (NAME_PROP).toString() };

		if (const auto c = dobj->getProperty (CHILDREN_PROP); c.isArray())
			for (const auto& child : *c.getArray())
				if (const auto childTree = fromVar (child); childTree.isValid())
					tree.appendChild (childTree, nullptr);

		const auto base64PropLen = static_cast<int> (std::strlen (BASE64_PROP));

		for (const auto& itr : dobj->getProperties())
		{
			const auto name = itr.name.toString();

			if (name == NAME_PROP || name == CHILDREN_PROP) continue;

			if (name.startsWith (BASE64_PROP))
			{
				tree.setProperty (name.substring (base64PropLen),
								  VariantConverter<MemoryBlock>::toVar (lemons::serializing::memoryBlockFromString (itr.value.toString())),
								  nullptr);
			}
			else
			{
				tree.setProperty (name, itr.value, nullptr);
			}
		}

		return tree;
	}

	return {};
}

var VariantConverter<ValueTree>::toVar (const ValueTree& tree)
{
	if (! tree.isValid())
		return {};

	DynamicObject obj;

	obj.setProperty (NAME_PROP, tree.getType().toString());

	Array<var> children;

	for (const auto child : tree)
		if (const auto childVar = toVar (child); ! childVar.isVoid())
			children.add (childVar);

	if (! children.isEmpty())
		obj.setProperty (CHILDREN_PROP, children);

	for (auto i = 0; i < tree.getNumProperties(); i++)
	{
		const auto name = tree.getPropertyName (i).toString();
		const auto val	= tree.getProperty (name, {});

		if (const auto* mb = val.getBinaryData())
		{
			obj.setProperty (BASE64_PROP + name,
							 VariantConverter<MemoryBlock>::toVar (*mb));

			continue;
		}

		// These types can't be stored as JSON!
		jassert (! val.isObject());
		jassert (! val.isMethod());
		jassert (! val.isArray());

		obj.setProperty (name, val.toString());
	}

	return { obj.clone().get() };
}

}  // namespace juce
