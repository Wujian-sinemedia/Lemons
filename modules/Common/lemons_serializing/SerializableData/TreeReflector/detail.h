#pragma once

#include <lemons_serializing/Serializing/Specializations/containers/Juce_containers.h>
#include <lemons_serializing/Serializing/Specializations/containers/STL_containers.h>

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
    using namespace serializing;

    if constexpr (! std::is_const< Type >())
    {
        if constexpr (std::is_base_of< SerializableData, Type >())
            loadDataChild (propertyName, object);
        else if constexpr (std::is_same< ValueTree, Type >())
            loadValueTree (propertyName, object);
        else if constexpr (isContainer< Type >() || isMap< Type >())
            loadContainer (propertyName, object);
        else
            loadObject (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::save (const String& propertyName, Type& object)
{
    using namespace serializing;

    if constexpr (std::is_base_of< SerializableData, Type >())
        saveDataChild (propertyName, object);
    else if constexpr (std::is_same< ValueTree, Type >())
        saveValueTree (propertyName, object);
    else if constexpr (isContainer< Type >() || isMap< Type >())
        saveContainer (propertyName, object);
    else
        saveObject (propertyName, object);
}

template < typename Type >
Type TreeReflector::toEnum (const juce::var& var) const
{
    return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
}

template < typename Type >
juce::var TreeReflector::fromEnum (Type value) const
{
    return static_cast< int > (static_cast< std::underlying_type_t< Type > > (value));
}

template < typename Type >
void TreeReflector::loadObject (const String& propertyName, Type& object)
{
    if (! tree.hasProperty (propertyName))
        return;

    const juce::var& var = tree.getProperty (propertyName);

    if constexpr (std::is_enum< Type >())
        object = toEnum< Type > (var);
    else
        object = serializing::fromVar< Type > (var);
}

template < typename Type >
void TreeReflector::saveObject (const String& propertyName, Type& object)
{
    juce::var var;

    if constexpr (std::is_enum< Type >())
        var = fromEnum (object);
    else
        var = serializing::toVar (object);

    tree.setProperty (propertyName, var, nullptr);
}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    const auto child = tree.getChildWithName (propertyNameToContainerName (propertyName));
    if (! child.isValid()) return;

    TreeLoader ref {child};

    if constexpr (serializing::isMap< ContainerType >())
        ref.addMap (container, propertyName);
    else
        ref.addContainer (container, propertyName);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    ValueTree child {propertyNameToContainerName (propertyName)};

    TreeSaver ref {child};

    if constexpr (serializing::isMap< ContainerType >())
        ref.addMap (container, propertyName);
    else
        ref.addContainer (container, propertyName);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

template < class ContainerType >
void TreeReflector::addContainer (ContainerType& container, const String& propertyName)
{
    if (isLoading())
    {
        serializing::getInterfaceForContainer (container)->resize (getNumElementsOfType (propertyName,
                                                                                         getRawDataTree()));
    }

    int index = 1;

    for (auto& element : container)
    {
        add (makePropertyNameForElement (propertyName, index++),
             element);
    }
}

template < class MapType >
void TreeReflector::addMap (MapType& map, const String& propertyName)
{
}

}  // namespace lemons
