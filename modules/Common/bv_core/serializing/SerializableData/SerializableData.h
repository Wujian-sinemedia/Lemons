
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
    void loadDataChild (const String& propertyName, SerializableData& data);

    ValueTree  tree;
    const bool loadingData;
};


struct SerializableData
{
    SerializableData (juce::Identifier identifier = "Data");
    virtual ~SerializableData() = default;

    SerializableData& operator= (SerializableData& other);

    ValueTree serialize()
    {
        return serialize (dataIdentifier);
    }

    ValueTree serialize (juce::Identifier treeID);

    void deserialize (const ValueTree& tree);

    const juce::Identifier& getDataIdentifier() const { return dataIdentifier; }

private:
    virtual void serialize (TreeReflector& ref) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
};


}  // namespace bav
