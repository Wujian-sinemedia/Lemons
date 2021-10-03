#pragma once

#include "helpers.h"

namespace lemons
{
namespace serializing
{
/** Converts a juce::var to a specified enum type. */
template < typename Type >
Type toEnum (const juce::var& var)
{
    return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
}


/** Converts an enum type to a juce::var. */
template < typename Type >
juce::var fromEnum (Type value)
{
    return static_cast< int > (static_cast< std::underlying_type_t< Type > > (value));
}


}  // namespace serializing

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
    using namespace serializing::TreeReflectorHelpers;

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
    using namespace serializing::TreeReflectorHelpers;

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
void TreeReflector::loadObject (const String& propertyName, Type& object)
{
    if (! tree.hasProperty (propertyName))
        return;

    const juce::var& var = tree.getProperty (propertyName);

    if constexpr (std::is_enum< Type >())
        object = serializing::toEnum< Type > (var);
    else
        object = serializing::fromVar< Type > (var);
}

template < typename Type >
void TreeReflector::saveObject (const String& propertyName, Type& object)
{
    juce::var var;

    if constexpr (std::is_enum< Type >())
        var = serializing::fromEnum (object);
    else
        var = serializing::toVar (object);

    tree.setProperty (propertyName, var, nullptr);
}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    using namespace serializing::TreeReflectorHelpers;

    const auto child = tree.getChildWithName (propertyNameToContainerName (propertyName));
    if (! child.isValid()) return;

    TreeLoader ref {child};

    if constexpr (serializing::isMap< ContainerType >())
        addMap (ref, container, propertyName);
    else
        addContainer (ref, container, propertyName);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    using namespace serializing::TreeReflectorHelpers;

    ValueTree child {propertyNameToContainerName (propertyName)};

    TreeSaver ref {child};

    if constexpr (serializing::isMap< ContainerType >())
        addMap (ref, container, propertyName);
    else
        addContainer (ref, container, propertyName);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

}  // namespace lemons
