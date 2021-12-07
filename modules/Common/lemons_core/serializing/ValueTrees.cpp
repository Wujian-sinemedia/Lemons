constexpr auto VT_NAME_PROP     = "_name";
constexpr auto VT_CHILDREN_PROP = "_children";
constexpr auto VT_BASE64_PROP   = "_base64:";


namespace juce
{

ValueTree VariantConverter<ValueTree>::fromVar (const var& obj)
{
	if (auto* dobj = obj.getDynamicObject(); dobj->hasProperty (VT_NAME_PROP))
	{
		ValueTree tree { dobj->getProperty (VT_NAME_PROP).toString() };

		if (const auto c = dobj->getProperty (VT_CHILDREN_PROP); c.isArray())
			for (const auto& child : *c.getArray())
				if (const auto childTree = fromVar (child); childTree.isValid())
					tree.appendChild (childTree, nullptr);

		const auto base64PropLen = static_cast<int> (std::strlen (VT_BASE64_PROP));

		for (auto itr : dobj->getProperties())
		{
			const auto name = itr.name.toString();

			if (name == VT_NAME_PROP || name == VT_CHILDREN_PROP) continue;

			if (name.startsWith (VT_BASE64_PROP))
			{
				MemoryBlock mb;

				if (mb.fromBase64Encoding (itr.value.toString()))
					tree.setProperty (name.substring (base64PropLen), var (mb), nullptr);
			}
			else
			{
				tree.setProperty (name, juce::var (itr.value), nullptr);
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

	obj.setProperty (VT_NAME_PROP, tree.getType().toString());

	Array<var> children;

	for (const auto child : tree)
		if (const auto childVar = toVar (child); ! childVar.isVoid())
			children.add (childVar);

	if (! children.isEmpty())
		obj.setProperty (VT_CHILDREN_PROP, children);

	for (int i = 0; i < tree.getNumProperties(); i++)
	{
		const auto name = tree.getPropertyName (i).toString();
		const auto val  = tree.getProperty (name, {});

		if (const auto* mb = val.getBinaryData())
		{
			obj.setProperty (VT_BASE64_PROP + name, mb->toBase64Encoding());
			continue;
		}

		// These types can't be stored as JSON!
		jassert (! val.isObject());
		jassert (! val.isMethod());
		jassert (! val.isArray());

		obj.setProperty (name, val.toString());
	}

	return juce::var (&obj);
}

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::serializing
{

String valueTreeToJSON (const ValueTree& tree)
{
	return juce::JSON::toString (juce::VariantConverter<ValueTree>::toVar (tree));
}

ValueTree valueTreeFromJSON (const String& jsonText)
{
	return juce::VariantConverter<ValueTree>::fromVar (juce::JSON::parse (jsonText));
}

}  // namespace lemons::serializing
