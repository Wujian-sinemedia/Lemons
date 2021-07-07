#pragma once

namespace bav
{

template < typename Type >
void TreeReflector::add (const String& propertyName, Type& object)
{
    if constexpr (std::is_pointer< Type >())
    {
        if constexpr (! std::is_null_pointer< Type >())
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

namespace TreeReflectorHelpers
{

template < typename ContainerType >
void addContainer (TreeReflector& child, const String& propertyName, ContainerType& container)
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

}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    const auto child = tree.getChildWithName (propertyName + "s");
    if (! child.isValid()) return;
    
    TreeReflector ref {child, true};
    
    TreeReflectorHelpers::addContainer (ref, propertyName, container);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    ValueTree     child {propertyName + "s"};
    TreeReflector ref {child, false};
    
    TreeReflectorHelpers::addContainer (ref, propertyName, container);
    
    tree.appendChild (ref.getRawDataTree(), nullptr);
}

}
