#pragma once

#include "helpers.h"

namespace bav
{
template < typename Type >
void TreeReflector::add (const String& propertyName, Type& object)
{
    if constexpr (std::is_pointer< Type >())
    {
        if constexpr (! std::is_null_pointer< Type >())
            if (object != nullptr)
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
    if constexpr (! std::is_const< Type >())
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
    if constexpr (isSerializable< Type >())
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

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    const auto child = tree.getChildWithName (propertyName + "s");
    if (! child.isValid()) return;

    TreeReflector ref {child, true};
    
    container.clear();
    container.resize (TreeReflectorHelpers::getNumElementsOfType (propertyName, child));
    
    ref.addContainer (container, propertyName);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    ValueTree     child {propertyName + "s"};
    TreeReflector ref {child, false};
    
    ref.addContainer (container, propertyName);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

template < class ContainerType >
void TreeReflector::addContainer (ContainerType& container, const String& propertyName)
{
    int index = 0;
    
    for (auto& element : container)
        add (TreeReflectorHelpers::makePropertyNameForElement (propertyName, index),
             element);
}

}  // namespace bav
