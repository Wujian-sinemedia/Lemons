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


struct TreeReflector final
{
    explicit TreeReflector (ValueTree treeToUse, bool loading);

    bool isLoading() const;
    
    ValueTree& getRawDataTree();

    template < typename Type >
    void add (const String& propertyName, Type& object)
    {
        if (loadingData)
            load (propertyName, object);
        else
            save (propertyName, object);
    }

private:
    template<typename Type>
    void save (const String& propertyName, Type& object)
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
    
    template<typename Type>
    void load (const String& propertyName, Type& object)
    {
        if constexpr (std::is_const_v< Type >)
            return;
        
        if constexpr (std::is_base_of_v< SerializableData, Type >)
            loadDataChild (propertyName, object);
        else
            replaceObjectFromTree (propertyName, object);
    }
    
    template<typename Type>
    void replaceObjectFromTree (const String& propertyName, Type& object)
    {
        if (tree.hasProperty (propertyName))
            object = fromVar< Type > (tree.getProperty (propertyName));
    }
    
    void saveDataChild (const String& propertyName, SerializableData& data);
    void loadDataChild (const String& propertyName, SerializableData& data) const;

    ValueTree  tree;
    const bool loadingData;
};

}  // namespace bav
