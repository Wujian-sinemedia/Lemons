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
    static_assert (std::is_default_constructible< Type >(),
                   "Your type must be default constructable to use TreeReflector::addLambdaSet()!");

    static_assert (! std::is_pointer< Type >(),
                   "You can use add() with pointer types, but not the lambda version!");

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

/*----------------------------------------------------------------------------------------------------------------------------------*/

template < typename Type >
void TreeReflector::load (const String& propertyName, Type& object)
{
    if constexpr (! std::is_const< Type >())
    {
        if constexpr (isSubtree< Type >())
            loadSubtree (propertyName, object);
        else
            loadObject (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::save (const String& propertyName, Type& object)
{
    if constexpr (isSubtree< Type >())
        saveSubtree (propertyName, object);
    else
        saveObject (propertyName, object);
}

/*----------------------------------------------------------------------------------------------------------------------------------*/

template < typename Type >
void TreeReflector::loadObject (const String& propertyName, Type& object)
{
    static_assert (std::is_assignable< Type&, Type >(),
                   "Your type must be assignable to add with TreeReflector!");

    if (! tree.hasProperty (propertyName))
        return;

    const juce::var& var = tree.getProperty (propertyName);

    auto convertFromVar = [&]() -> Type
    {
        if constexpr (std::is_enum< Type >())
            return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
        else
            return serializing::fromVar< Type > (var);
    };

    object = convertFromVar();
}

template < typename Type >
void TreeReflector::saveObject (const String& propertyName, const Type& object)
{
    auto convertToVar = [&]() -> juce::var
    {
        if constexpr (std::is_enum< Type >())
            return static_cast< int > (static_cast< std::underlying_type_t< Type > > (object));
        else
            return serializing::toVar (object);
    };

    tree.setProperty (propertyName, convertToVar(), nullptr);
}

/*----------------------------------------------------------------------------------------------------------------------------------*/

template < class Type >
void TreeReflector::loadSubtree (const String& propertyName, Type& object)
{
    const auto child = tree.getChildWithName (propertyName);
    if (! child.isValid()) return;

    TreeLoader ref {child};

    ref.addSubtree (object);
}

template < class Type >
void TreeReflector::saveSubtree (const String& propertyName, Type& object)
{
    ValueTree child {propertyName};

    TreeSaver ref {child};

    ref.addSubtree (object);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

template < class Type >
void TreeReflector::addSubtree (Type& object)
{
    if constexpr (std::is_same< ValueTree, Type >())
        addValueTree (object);
    else if constexpr (std::is_base_of< SerializableData, Type >())
        addSerializableData (object);
    else if constexpr (serializing::isMap< Type >())
        addMap (object);
    else
        addContainer (object);
}

/*----------------------------------------------------------------------------------------------------------------------------------*/

template < class ContainerType >
void TreeReflector::addContainer (ContainerType& container)
{
    const auto singlePropertyName = [&]() -> String
    {
        const auto str = tree.getType().toString();

        if (str.endsWith ("s"))
            str.dropLastCharacters (1);

        return str + "_";
    }();

    auto makePropertyNameForContainerElement = [&] (int index) -> String
    {
        return singlePropertyName + String (index);
    };

    auto getNumContainerElements = [&] (bool elementsAreContainers) -> int
    {
        const auto total = elementsAreContainers ? tree.getNumChildren() : tree.getNumProperties();

        juce::Array< String > names;

        for (int i = 1; i <= total; ++i)
            names.add (makePropertyNameForContainerElement (i));

        auto actualNum = total;

        for (int i = 0; i < total; ++i)
        {
            const auto test = elementsAreContainers ? tree.getChild (i).getType() : tree.getPropertyName (i);

            if (! names.contains (test.toString()))
                --actualNum;
        }

        return actualNum;
    };

    if (isLoading())
    {
        using ElementType = typename std::decay_t< decltype (container.begin()) >;

        using namespace serializing;

        getInterfaceForContainer (container)->resize (getNumContainerElements (isContainer< ElementType >()));
    }

    int index = 1;

    for (auto& element : container)
        add (makePropertyNameForContainerElement (index++),
             element);
}

template < class MapType >
void TreeReflector::addMap (MapType& map)
{
}

/*----------------------------------------------------------------------------------------------------------------------------------*/

template < typename Type >
constexpr bool TreeReflector::isSubtree()
{
    using namespace serializing;

    return isContainer< Type >() || isMap< Type >() || std::is_same< ValueTree, Type >() || std::is_base_of< SerializableData, Type >();
}

}  // namespace lemons
