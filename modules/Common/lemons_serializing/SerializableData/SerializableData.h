
#pragma once


namespace lemons
{
/**
    Base class for any C++ object that needs to be serialized to binary and back.
    This class is built on top of juce::ValueTree, and provides a convenient API for assigning and retrieving ValueTree properties when implementing a serializable class, and for easily saving and loading the state of a serialiable object with a single function call.
    @see TreeReflector
 */
struct SerializableData
{
    /**
        Creates a serializable object.
        The identifier argument is optional, and really only needed for an app/plugin's top-level state object; usually, when building a heirarchy of serializable objects that include other serializable objects, you'll use the TreeReflector::add() API, and the property name you pass to that function call will override the child serializable object's identifier.
        The only time this class uses the identifier passed at construction time is when you call serialize() with no parameters -- this will give you a ValueTree whose top-level type is the identifier passed to the serializable object's constructor.
     */
    SerializableData (juce::Identifier identifier = "Data");

    /** Destructor. */
    virtual ~SerializableData() = default;

    /** Default assignment operator that uses the serialize/deserialize API internally. */
    SerializableData& operator= (SerializableData& other);

    /**
        Serializes the state of this object and returns a ValueTree.
        Internally, this will create a TreeSaver object and call the virtual serialize(TreeReflector&) function.
        @see TreeSaver, TreeReflector
     */
    ValueTree serialize();

    /**
        Restores a previously saved state.
        Internally, this will create a TreeLoader object and call the virtual serialize(TreeReflector&) function.
        @param tree The saved state you wish to restore. This should usually be a ValueTree you got from a previous call to serialize(). If the state is incomplete, for a different kind of object, or otherwise corrupted, this function will only change the properties that it can successfully parse from the passed state. For example, if your serializable object uses properties named "apple", "orange", and "grape", and you pass a ValueTree with entries for "apple", "grapefruit", and "mango", then the "grapefruit" and "mango" properties will be ignored, and only the "apple" property will actually be loaded.
        @see TreeLoader, TreeReflector
     */
    void deserialize (const ValueTree& tree);

private:
    friend struct TreeReflector;

    /**
        Implement this function for your derived type's custom serializing logic.
        This function will be called for both loading and saving. If your object is being loaded, the TreeReflector passed to this function will be a TreeLoader; otherwise, it will be a TreeSaver.
        There are two main ways of implementing TreeReflector serialization logic.
        You can use TreeReflector::add() with normal data members:
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
        The add method also works for containers -- if it's a standard type or if you've implemented ContainerInterface, getInterfaceForContainer(), and isContainer, then calling TreeReflector::add() will create a sub-tree in your object's main ValueTree that represents this container, then it will step through the container using a range-based for loop, calling TreeReflector::add() on each element sequentially.
     
        You can also use TreeReflector::addLambdaSet() if you need to call functions of your object to save or load attributes:
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
        If your object requires more low-level control or complex logic, you can also use TreeReflector::getRawDataTree(), and you can query TreeReflector::isLoading() or TreeReflector::isSaving().
        @see TreeReflector
     */
    virtual void serialize (TreeReflector& ref) = 0;

    ValueTree saveToTree (const String& treeName);
    void      restoreFromTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
};

}  // namespace lemons
