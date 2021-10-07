#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <lemons_core/lemons_core.h>

namespace lemons
{
struct SerializableData;


/**
    Utility class that provides a convenient API for assigning data members or lambdas to ValueTree properties for use in serializing logic.
    You should never have to create one of these; the SerializableData class will internally create and use TreeLoaders and TreeSavers when you call its top-level load and save functions. \n
    \n
    Internally, this class uses the toVar() and fromVar() functions to encode various C++ types into ValueTree properties. Implement those two functions within the lemons::serializing namespace for any custom type you wish to easily serialize. \n
    \n
    As a general rule, you shouldn't need to implement toVar() and fromVar() for a type if that type inherits from SerializableData; and, conversely, you should implement toVar() and fromVar() for a type if it needs to be serialized and doesn't inherit from SerializableData. You should rarely, if ever, need both for a single type.
    @see SerializableData, serializing::toVar(), serializing::fromVar()
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
        - If the passed object is a ValueTree, it will be appended to this reflector's internal ValueTree using normal ValueTree methods. \n
        - If the passed object inherits from SerializableData, this will call serialize() on it and append the resulting ValueTree to the reflector's internal tree. \n
        - If the passed object is a container, this will create a subtree for the container and populate it by calling add() on each element of the container. This works recursively for multidimensional containers as well. \n
        - If the passed object is any other kind of C++ object, this will call serializing::toVar() to save the object to the ValueTree, or serializing::fromVar() to restore from the tree. \n
        \n
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
        \n
        @attention Be careful not to use duplicate property names in the serialization logic for a single object! \n
        Example -- what NOT to do:
        @code
        struct SomethingSerializable : lemons::SerializableData
        {
            int number;
            String string;
     
            void serialize (TreeReflector& ref) final
            {
                ref.add ("ThisWillBreak!", number);
                ref.add ("ThisWillBreak!", string);
            }
        };
        @endcode
        In the above example, the 'string' data will overwrite the 'number' data, because the programmer has chosen duplicate property names. \n
        \n
        Container types you wish to serialize with a single call to add() must fulfill the following: \n
        - have begin() and end() functions -- ie, are compatable with range-based for loops
        - have an implementation of serializing::ContainerInterface
        - have a specialization of serializing::getInterfaceForContainer()
        - have a specialization of serializing::isContainer that inherits std::true_type
        - The container's elements must themselves be valid calls to add() -- ie, either inherit from SerializableData or have serializing::toVar() and serializing::fromVar() implemented. \n
        \n
        Provided in Lemons are implementations for common STL containers and JUCE containers. You can implement the above requirements for this to work with any other custom type you create. \n
        \n
        For example:
        @code
        struct SomethingSerializable : lemons::SerializableData
        {
            std::vector<int> data;
     
            void serialize (TreeReflector& ref) final
            {
                ref.add ("Ints", data);
            }
        };
        @endcode
        This will create a sub-tree in your object's ValueTree named "Ints", with properties "Int_1", "Int_2", "Int_3"... etc. This works by iterating through each element of your container and calling add() on each of them.
     
        @param propertyName The property name associated with the piece of data this function call represents. \n
        - When adding a reference to a member encoded with toVar() and fromVar(), this is the name of the ValueTree property the encoded object will be saved to and restored from in the reflector's internal ValueTree. \n
        - When adding a sub-tree object (such as when passing a child ValueTree, a child object that inherits from SerializableData, or a container), this is the name of the sub-tree that will be added to the reflector's internal tree, and will be populated with the child object's properties. \n
        - When adding a container, the container's individual element names are generated as following:
        @code
            std::vector<int> data {23, 16, 7};
     
            ref.add ("Points", data);
        @endcode
        This will create the following sub-tree object:
        @verbatim
            <Points>
                <Point_1 = 23>
                <Point_2 = 16>
                <Point_3 = 7>
        @endverbatim
        I do not recommend attempting to internationalize property names. \n \n
        Take care to avoid duplicating property names for multiple data members of the same serializable object!
     
        @param object Reference to the data member you wish to load/save. \n
        This can be: \n
        - Any C++ object for which toVar() and fromVar() are implemented;
        - A juce::ValueTree;
        - Another object that inherits from SerializableData;
        - A container that has serializing::ContainerInterface, serializing::getInterfaceForContainer(), and serializing::isContainer implemented and whose elements are also valid calls to add()
     
        @see serializing::ContainerInterface, serializing::getInterfaceForContainer(), serializing::isContainer, serializing::toVar(), serializing::fromVar(), addLambdaSet()
     */
    template < typename Type >
    void add (const String& propertyName, Type& object);


    /** Binds lambda functions for saving and loading a certain value to and from a named ValueTree property.
        This follows the same semantics as add(), but provides the ability to fetch the value using a lambda, and call a lambda with the value retrieved from the tree, instead of providing a reference to an object. \n
        \n
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
                                        [&](int& newData){ setData (newData); });
            }
         };
         @endcode
        If you use this function with a container type, your lambdas must return/recieve the entire container by value. I recommend sticking to add() (or implementing completely custom logic) for containers.
     
        @param propertyName The name of the ValueTree property that this data corresponds to. See the docs for add().
     
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
        @attention Handle with care! The tree this function returns may already contain sub-nodes representing other serialized children of whatever object is serializing you at the moment...
    */
    ValueTree& getRawDataTree();

private:
    template < typename Type >
    void load (const String& propertyName, Type& object);

    template < typename Type >
    void save (const String& propertyName, Type& object);

    /*------------------------------------------*/

    template < typename Type >
    void loadObject (const String& propertyName, Type& object);

    template < typename Type >
    void saveObject (const String& propertyName, const Type& object);

    /*------------------------------------------*/

    template < class Type >
    void loadSubtree (const String& propertyName, Type& object);

    template < class Type >
    void saveSubtree (const String& propertyName, Type& object);

    /*------------------------------------------*/

    template < class Type >
    void addSubtree (Type& object);

    void addValueTree (ValueTree& data);

    void addSerializableData (SerializableData& data);

    template < class ContainerType >
    void addContainer (ContainerType& container);

    template < class MapType >
    void addMap (MapType& map);

    /*------------------------------------------*/

    template < typename Type >
    static constexpr bool isSubtree();

    /*------------------------------------------*/

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

#include "detail.h"
