#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <lemons_core/lemons_core.h>

namespace lemons
{
struct SerializableData;


/**
    Utility class that provides a convenient API for assigning data members or lambdas to ValueTree properties for use in serializing logic.
    You should never have to create one of these, the SerializableData class will internally create and use TreeLoaders and TreeSavers when you call its top-level load and save functions.
    Internally, this class uses the toVar() and fromVar() functions to encode various C++ types into ValueTree properties. Implement those two functions within the lemons::serializing namespace for any custom type you wish to easily serialize.
    @see SerializableData, serializing::toVar, serializing::fromVar
 */
struct TreeReflector
{
    /** Creates a TreeReflector.
        Whether this reflector is loading or saving, it must be given a ValueTree to reference.
     */
    explicit TreeReflector (const ValueTree& treeToUse);

    /** Destructor. */
    virtual ~TreeReflector() = default;

    /** Returns true if this reflector is loading state. */
    virtual bool isLoading() const = 0;

    /** Returns true if this reflector is saving state. */
    bool isSaving() const;


    /** Binds a named ValueTree property to a data member of your object.
        If the passed object is a ValueTree, it will be appended to this reflector's internal ValueTree using normal ValueTree methods.
        If the passed object inherits from SerializableData, this will call serialize() on it and append the resulting ValueTree to the reflector's internal tree.
        If the passed object is a container, this will create a subtree for the container and populate it by calling add() on each element of the container. This works recursively for multidimensional containers as well.
        If the passed object is any other kind of C++ object, this will call serializing::toVar() to save the object to the ValueTree, or serializing::fromVar() to restore from the tree.
        Be careful not to use duplicate property names in the serialization logic for a single object!
        For example:
        @code
        struct SomethingSerializable : lemons::SerializableData
        {
            // we want to save this integer with our program's state
            int data;
     
            void serialize (TreeReflector& ref) final
            {
                // simply choose a property name -- and that's it!
                ref.add ("MyData", data);
            }
        };
        @endcode
        Container types you wish to serialize with a single call to add() must fulfill the following:
        - have begin() and end() functions -- ie, are compatable with range-based for loops
        - have an implementation of ContainerInterface
        - have a specialization of getInterfaceForContainer()
        - have a specialization of isContainer that inherits std::true_type.
        Provided in Lemons are implementations for common STL containers, as well as juce::Array and juce::OwnedArray. You can implement the above requirements for this to work with any other custom type you create. This should work recursively for multidimensional containers as well.
        When calling add() with a container, the property name you pass should be the name of a single element in the container.
        For example:
        @code
        struct SomethingSerializable : lemons::SerializableData
        {
            std::vector<int> data;
     
            void serialize (TreeReflector& ref) final
            {
                ref.add ("Int", data);
            }
        };
        @endcode
        This will create a sub-tree in your object's ValueTree named "Ints", with properties "Int_1", "Int_2", "Int_3", etc. This works by iterating through each element of your container and calling add() on each of them.
     
        @param propertyName The name of the ValueTree property that this data member corresponds to. When saving, the juce::var-encoded version of the data member will be written to the tree using this property name. When loading, if the tree has a property with this name, it will be converted from juce::var back into your type and assigned to the data member. When adding a container, this should be the name of a single container element.
     
        @param object Reference to the data member you wish to load/save.
     
        @see ContainerInterface, getInterfaceForContainer(), isContainer, serializing::toVar(), serializing::fromVar(), addLambdaSet()
     */
    template < typename Type >
    void add (const String& propertyName, Type& object);


    /** Binds lambda functions for saving and loading a certain value to and from a named ValueTree property.
        This follows the same semantics as add(), but provides the ability to fetch the value using a lambda, and call a lambda with the value retrieved from the tree, instead of providing a reference to an object.
        Be careful not to use duplicate property names in the serialization logic for a single object!
        For example:
         @code
         struct SomethingSerializable : lemons::SerializableData
         {
            void setData (int newData)
            {
                data = newData;
                // do something else with side effects...
            }
         
        private:
            // we still want to save this integer with our program's state, but I want to call setData() when it's being loaded...
            int data;
         
            void serialize (TreeReflector& ref) final
            {
                // you can specify lambdas for retrieving and saving a property's value as a specified type (int)
                ref.addLambdaSet<int> ("MyData",
                                        [&]{ return data; },
                                        [&](int newData){ setData (newData); });
            }
         };
         @endcode
        If you use this function with a container type, your lambdas must return/recieve the entire container by value. I recommend sticking to add() (or implementing completely custom logic) for containers.
     
        @param propertyName The name of the ValueTree property that this data corresponds to. When saving, the juce::var-encoded version of the object returned from the saveToTree function will be written to the tree using this property name. When loading, if the tree has a property with this name, it will be converted from juce::var back into your type and passed to the loadFromTree function.
     
        @param saveToTree This lambda should return the current value of the data you wish to save.
     
        @param loadFromTree This lambda will be called with the new value for this data when a new state is loaded.
     
        @see add(), serializing::toVar(), serializing::fromVar()
     */
    template < typename Type >
    void addLambdaSet (const String& propertyName,
                       std::function< Type() >
                           saveToTree,
                       std::function< void (Type&) >
                           loadFromTree);


    /** Returns a reference to this reflector's root ValueTree.
        Handle with care! The tree this function returns may already contain sub-nodes representing other serialized children of whatever object is serializing you at the moment...
    */
    ValueTree& getRawDataTree();

private:
    template < typename Type >
    void load (const String& propertyName, Type& object);

    template < typename Type >
    void save (const String& propertyName, Type& object);

    template < class ContainerType >
    void loadContainer (const String& propertyName, ContainerType& container);

    template < class ContainerType >
    void saveContainer (const String& propertyName, ContainerType& container);

    void loadDataChild (const String& propertyName, SerializableData& data) const;
    void saveDataChild (const String& propertyName, SerializableData& data);

    ValueTree tree;
};


/**
    Subclass of TreeReflector used for loading state.
    @see TreeReflector
 */
struct TreeLoader : TreeReflector
{
    using TreeReflector::TreeReflector;

    bool isLoading() const final;
};


/**
    Subclass of TreeReflector used for saving state.
    @see TreeReflector
 */
struct TreeSaver : TreeReflector
{
    using TreeReflector::TreeReflector;

    bool isLoading() const final;
};

}  // namespace lemons

#include "detail/detail.h"
