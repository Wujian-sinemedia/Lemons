#pragma once

namespace bav
{
template < typename Type >
juce::var toVar (Type& object)
{
    return {object};
}

template < typename Type >
Type fromVar (juce::var var)
{
    return {var};
}


struct SerializableData;


struct TreeReflector
{
    explicit TreeReflector (ValueTree treeToUse, bool loading);

    bool isLoading() const;

    template < typename Type >
    void add (const String& propertyName, Type& object)
    {
        if (! loadingData)
        {
            if constexpr (std::is_base_of_v< SerializableData, Type > && ! std::is_const_v< Type >)
            {
                saveDataChild (propertyName, object);
            }
            else
            {
                tree.setProperty (propertyName, toVar< Type > (object), nullptr);
            }
        }
        else if constexpr (! std::is_const_v< Type >)
        {
            if constexpr (std::is_base_of_v< SerializableData, Type >)
            {
                loadDataChild (propertyName, object);
            }
            else
            {
                if (tree.hasProperty (propertyName))
                    object = fromVar< Type > (tree.getProperty (propertyName));
            }
        }
    }

    ValueTree& getRawDataTree();

private:
    void saveDataChild (const String& propertyName, SerializableData& data);

    void loadDataChild (const String& propertyName, SerializableData& data) const;

    ValueTree  tree;
    const bool loadingData;
};

}  // namespace bav
