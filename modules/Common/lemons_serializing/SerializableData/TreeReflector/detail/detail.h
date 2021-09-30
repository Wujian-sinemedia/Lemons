#pragma once

#include "helpers.h"

namespace lemons
{
template < typename Type >
void TreeReflector::add (const String& propertyName, Type& object)
{
    if constexpr (std::is_pointer< Type >())
    {
        if (object != nullptr)
            add (propertyName, *object);
    }
    else
    {
        if (isLoading())
            load (propertyName, object);
        else
            save (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::addLambdaSet (const String& propertyName,
                                  std::function< Type() >
                                      saveToTree,
                                  std::function< void (Type&) >
                                      loadFromTree)
{
    if (isLoading())
    {
        Type object;
        load (propertyName, object);
        loadFromTree (object);
    }
    else
    {
        auto object = saveToTree();
        save (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::load (const String& propertyName, Type& object)
{
    using namespace serializing::TreeReflectorHelpers;

    if constexpr (! std::is_const< Type >())
    {
        if constexpr (isSerializable< Type >())
            loadDataChild (propertyName, object);
        else if constexpr (isContainer< Type >())
            loadContainer (propertyName, object);
        else if constexpr (std::is_same< ValueTree, Type >())
            loadValueTree (tree, propertyName, object);
        else
            loadObject (tree, propertyName, object);
    }
}

template < typename Type >
void TreeReflector::save (const String& propertyName, Type& object)
{
    using namespace serializing::TreeReflectorHelpers;

    if constexpr (isSerializable< Type >())
        saveDataChild (propertyName, object);
    else if constexpr (isContainer< Type >())
        saveContainer (propertyName, object);
    else if constexpr (std::is_same< ValueTree, Type >())
        saveValueTree (tree, propertyName, object);
    else
        saveObject (tree, propertyName, object);
}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    using namespace serializing::TreeReflectorHelpers;

    const auto child = tree.getChildWithName (propertyNameToContainerName (propertyName));
    if (! child.isValid()) return;

    TreeLoader ref {child};

    addContainer (ref, container, propertyName);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    using namespace serializing::TreeReflectorHelpers;

    ValueTree child {propertyNameToContainerName (propertyName)};

    TreeSaver ref {child};

    addContainer (ref, container, propertyName);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

}  // namespace lemons
