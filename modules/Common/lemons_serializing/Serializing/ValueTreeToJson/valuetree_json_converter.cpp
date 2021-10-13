namespace lemons::serializing
{
constexpr auto NAME_PROP     = "_name";
constexpr auto CHILDREN_PROP = "_children";
constexpr auto BASE64_PROP   = "_base64:";


[[nodiscard]] static inline juce::var valueTreeToVar (const ValueTree& v)
{
    if (! v.isValid())
        return {};

    juce::DynamicObject obj;

    obj.setProperty (NAME_PROP, v.getType().toString());

    juce::Array< juce::var > children;

    for (auto c : v)
        children.add (valueTreeToVar (c));

    if (! children.isEmpty())
        obj.setProperty (CHILDREN_PROP, children);

    for (int i = 0; i < v.getNumProperties(); i++)
    {
        const auto name = v.getPropertyName (i).toString();
        const auto val  = v.getProperty (name, {});

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

[[nodiscard]] String valueTreeToJSON (const ValueTree& v)
{
    return juce::JSON::toString (valueTreeToVar (v));
}


//==============================================================================
//==============================================================================


[[nodiscard]] static inline ValueTree valueTreefromVar (const juce::var& obj)
{
    if (auto* dobj = obj.getDynamicObject(); dobj->hasProperty (NAME_PROP))
    {
        ValueTree v {dobj->getProperty (NAME_PROP).toString()};

        if (const auto c = dobj->getProperty (CHILDREN_PROP); c.isArray())
            for (const auto& child : *c.getArray())
                if (const auto childTree = valueTreefromVar (child); childTree.isValid())
                    v.appendChild (childTree, nullptr);

        const auto base64PropLen = static_cast< int > (std::strlen (BASE64_PROP));

        for (auto itr : dobj->getProperties())
        {
            const auto name = itr.name.toString();

            if (name == NAME_PROP || name == CHILDREN_PROP) continue;

            if (name.startsWith (BASE64_PROP))
            {
                juce::MemoryBlock mb;

                if (mb.fromBase64Encoding (itr.value.toString()))
                    v.setProperty (name.substring (base64PropLen),
                                   juce::var (mb), nullptr);
            }
            else
            {
                v.setProperty (name, juce::var (itr.value), nullptr);
            }
        }

        return v;
    }

    return {};
}

[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText)
{
    return valueTreefromVar (juce::JSON::parse (jsonText));
}


}  // namespace lemons::serializing
