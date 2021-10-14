namespace lemons::serializing
{
constexpr auto NAME_PROP     = "_name";
constexpr auto CHILDREN_PROP = "_children";
constexpr auto BASE64_PROP   = "_base64:";


[[nodiscard]] static inline juce::var valueTreeToVar (const ValueTree& tree)
{
    if (! tree.isValid())
        return {};

    juce::DynamicObject obj;

    obj.setProperty (NAME_PROP, tree.getType().toString());

    juce::Array< juce::var > children;

    for (auto child : tree)
        if (const auto childVar = valueTreeToVar (child); ! childVar.isVoid())
            children.add (childVar);

    if (! children.isEmpty())
        obj.setProperty (CHILDREN_PROP, children);

    for (int i = 0; i < tree.getNumProperties(); i++)
    {
        const auto name = tree.getPropertyName (i).toString();
        const auto val  = tree.getProperty (name, {});

        if (const auto mb = val.getBinaryData())
        {
            obj.setProperty (BASE64_PROP + name, mb->toBase64Encoding());
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

[[nodiscard]] String valueTreeToJSON (const ValueTree& tree)
{
    return juce::JSON::toString (valueTreeToVar (tree));
}


//==============================================================================
//==============================================================================


[[nodiscard]] static inline ValueTree valueTreefromVar (const juce::var& obj)
{
    if (auto* dobj = obj.getDynamicObject(); dobj->hasProperty (NAME_PROP))
    {
        ValueTree tree {dobj->getProperty (NAME_PROP).toString()};

        if (const auto c = dobj->getProperty (CHILDREN_PROP); c.isArray())
            for (const auto& child : *c.getArray())
                if (const auto childTree = valueTreefromVar (child); childTree.isValid())
                    tree.appendChild (childTree, nullptr);

        const auto base64PropLen = static_cast< int > (std::strlen (BASE64_PROP));

        for (auto itr : dobj->getProperties())
        {
            const auto name = itr.name.toString();

            if (name == NAME_PROP || name == CHILDREN_PROP) continue;

            if (name.startsWith (BASE64_PROP))
            {
                juce::MemoryBlock mb;

                if (mb.fromBase64Encoding (itr.value.toString()))
                    tree.setProperty (name.substring (base64PropLen), juce::var (mb), nullptr);
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

[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText)
{
    return valueTreefromVar (juce::JSON::parse (jsonText));
}


}  // namespace lemons::serializing
