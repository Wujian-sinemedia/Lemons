#pragma once

#include <bv_core/bv_core.h>

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

template < typename Type >
constexpr bool isSerializable()
{
    return std::is_base_of_v< SerializableData, Type >;
}


struct TreeReflector final
{
    explicit TreeReflector (ValueTree treeToUse, bool loading);

    bool isLoading() const;
    bool isSaving() const;

    ValueTree& getRawDataTree();

    template < typename Type >
    void add (const String& propertyName, Type& object);

private:
    template < typename Type >
    void load (const String& propertyName, Type& object);

    template < typename Type >
    void save (const String& propertyName, Type& object);

    template < typename Type >
    void loadObject (const String& propertyName, Type& object) const;

    template < typename Type >
    void saveObject (const String& propertyName, Type& object);

    template < class ContainerType >
    void loadContainer (const String& propertyName, ContainerType& container);

    template < class ContainerType >
    void saveContainer (const String& propertyName, ContainerType& container);

    void loadDataChild (const String& propertyName, SerializableData& data) const;
    void saveDataChild (const String& propertyName, SerializableData& data);

    ValueTree  tree;
    const bool loadingData;
};


/*--------------------------------------------------------------------------------------------*
 _____  ______ _______       _____ _
|  __ \|  ____|__   __|/\   |_   _| |
| |  | | |__     | |  /  \    | | | |
| |  | |  __|    | | / /\ \   | | | |
| |__| | |____   | |/ ____ \ _| |_| |____ _ _ _
|_____/|______|  |_/_/    \_\_____|______(_|_|_)
--------------------------------------------------------------------------------------------*/

template < typename Type >
void TreeReflector::add (const String& propertyName, Type& object)
{
    if constexpr (std::is_pointer_v< Type >)
    {
        if constexpr (! std::is_null_pointer_v< Type >)
            add (propertyName, *object);
    }
    else
    {
        if (loadingData)
            load (propertyName, object);
        else
            save (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::load (const String& propertyName, Type& object)
{
    if constexpr (! std::is_const_v< Type >)
    {
        if constexpr (isSerializable< Type >())
            loadDataChild (propertyName, object);
        else if constexpr (is_container< Type >())
            loadContainer (propertyName, object);
        else
            loadObject (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::save (const String& propertyName, Type& object)
{
    if constexpr (isSerializable< Type >() && ! std::is_const_v< Type >)
        saveDataChild (propertyName, object);
    else if constexpr (is_container< Type >())
        saveContainer (propertyName, object);
    else
        saveObject (propertyName, object);
}

template < typename Type >
void TreeReflector::loadObject (const String& propertyName, Type& object) const
{
    if (tree.hasProperty (propertyName))
        object = fromVar< Type > (tree.getProperty (propertyName));
}

template < typename Type >
void TreeReflector::saveObject (const String& propertyName, Type& object)
{
    tree.setProperty (propertyName, toVar< Type > (object), nullptr);
}

template < typename ContainerType >
void addContainerInternal (TreeReflector& child, const String& propertyName, ContainerType& container)
{
    int index = 0;

    const auto makePropertyNameForElement = [&]
    {
        const auto name = propertyName + "_" + String (index);
        ++index;
        return name;
    };

    for (auto& element : container)
        child.add (makePropertyNameForElement(), element);
}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    const auto child = tree.getChildWithName (propertyName + "s");
    if (! child.isValid()) return;

    TreeReflector ref {child, true};

    addContainerInternal (ref, propertyName, container);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    ValueTree     child {propertyName + "s"};
    TreeReflector ref {child, false};

    addContainerInternal (ref, propertyName, container);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

}  // namespace bav
