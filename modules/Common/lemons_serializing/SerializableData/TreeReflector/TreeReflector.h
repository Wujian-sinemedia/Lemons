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
        Uses serializing::toVar() to save to the tree, and serializing::fromVar() to restore from the tree.
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
        This also works for containers that have begin() and end() functions; for containers, the property name you pass should be the name of a single element in the container.
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
        This will create a sub-tree in your object's ValueTree named "Ints", with properties "Int_1", "Int_2", "Int_3", etc. This works by iterating through each element of your container and calling add() on each of them -- so the type of the elements of your container must have toVar() and fromVar() implemented.
     
        @param propertyName The name of the ValueTree property that this data member corresponds to. When saving, the juce::var-encoded version of the data member will be written to the tree using this property name. When loading, if the tree has a property with this name, it will be converted from juce::var back into your type and assigned to the data member. When adding a container, this should be the name of a single container element.
     
        @param object Reference to the data member you wish to load/save.
     
        @see serializing::toVar(), serializing::fromVar()
     */
    template < typename Type >
    void add (const String& propertyName, Type& object);


    /** Binds lambda functions for saving and loading a certain value to and from a named ValueTree property.
        This works similarly to add(), but provides the ability to fetch the value using a lambda, and call a lambda with the value retrieved from the tree.
        Internally, uses serializing::toVar() to save to the tree, and serializing::fromVar() to restore from the tree.
        Be careful not to use duplicate property names in the serialization logic for a single object!
        For example:
         @code
         struct SomethingSerializable : lemons::SerializableData
         {
            // maybe this function has side effects?
            void setData (int newData);
         
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
        If you use this function with a container type, your lambdas must return/recieve the entire container by value. I recommend sticking to add() (or implementing custom logic) for containers.
     
        @param propertyName The name of the ValueTree property that this data corresponds to. When saving, the juce::var-encoded version of the object returned from the saveToTree function will be written to the tree using this property name. When loading, if the tree has a property with this name, it will be converted from juce::var back into your type and passed to the loadFromTree function.
     
        @param saveToTree This lambda should return the current value of the data you wish to save.
     
        @param loadFromTree This lambda will be called with the new value for this data when a new state is loaded.
     
        @see serializing::toVar(), serializing::fromVar()
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
