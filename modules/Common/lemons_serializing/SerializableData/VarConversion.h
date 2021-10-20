#pragma once

#include <juce_core/juce_core.h>

namespace lemons::serializing
{
/**
    Implement this function and fromVar() for any custom type to allow easily serialization using TreeReflector::add().
    This function must save the complete state of your object either directly to a juce::var or to a type that is implicitly convertable to juce::var (ie, float, int, juce::String, etc.). \n \n
    This function must do the exact opposite of what your implementation of fromVar() for your type does. \n \n
    Usually what this boils down to is encoding your object into a juce::String in some way, which can then be implicitly converted to a juce::var. \n \n
    Several examples of implementations of this function can be found in lemons_serializing/Serializing/Specializations/VarSpecializations.cpp. \n \n
    As a general rule, you shouldn't need to implement toVar() and fromVar() for a type if that type inherits from SerializableData; and, conversely, you should implement toVar() and fromVar() for a type if it needs to be serialized and doesn't inherit from SerializableData. You should rarely, if ever, need both for a single type. If your type has multiple distinct pieces of data or steps to its serialization logic, it is usually preferable to have your type inherit from SerializableData.
    @code
    // example implementation:
    namespace lemons
    {
        template<>
        juce::var toVar (MyCustomType& object)
        {
            // implement your conversion logic here...
        }

        template<>
        MyCustomType fromVar (juce::var var)
        {
            // implement your conversion logic here...
        }
    }

    // example usage:
    struct SomethingSerializable : lemons::SerializableData
    {
        void serialize (TreeReflector& ref) final
        {
            // TreeReflector will internally call toVar to save your object and fromVar to load your object
            ref.add ("MyCustomData", data);
        }

        MyCustomType data;
    };
    @endcode

    @see fromVar(), SerializableData, TreeReflector
 */
template <typename Type>
juce::var toVar (const Type& object)
{
	return { object };
}


/**
    Implement this function and toVar() for any custom type to allow easily serialization using TreeReflector::add().
    This function must load the complete state of your object from a juce::var. Your object type must be default-constructable. \n \n
    This function must do the exact opposite of what your implementation of toVar() for your type does. \n \n
    Usually what this boils down to is creating a default-constructed instance of your object, then setting properties or relevant data members programmatically based on parsing the encoded String returned from the var. \n \n
    Several examples of implementations of this function can be found in lemons_serializing/Serializing/Specializations/VarSpecializations.cpp. \n \n
    As a general rule, you shouldn't need to implement toVar() and fromVar() for a type if that type inherits from SerializableData; and, conversely, you should implement toVar() and fromVar() for a type if it needs to be serialized and doesn't inherit from SerializableData. You should rarely, if ever, need both for a single type. If your type has multiple distinct pieces of data or steps to its serialization logic, it is usually preferable to have your type inherit from SerializableData.
    @code
    // example implementation:
    namespace lemons
    {
        template<>
        juce::var toVar (MyCustomType& object)
        {
            // implement your conversion logic here...
        }

        template<>
        MyCustomType fromVar (juce::var var)
        {
            // implement your conversion logic here...
        }
    }

    // example usage:
    struct SomethingSerializable : lemons::SerializableData
    {
        void serialize (TreeReflector& ref) final
        {
            // TreeReflector will internally call toVar to save your object and fromVar to load your object
            ref.add ("MyCustomData", data);
        }

        MyCustomType data;
    };
    @endcode

    @see toVar(), SerializableData, TreeReflector
 */
template <typename Type>
Type fromVar (const juce::var& var)
{
	return { var };
}

}  // namespace lemons::serializing
