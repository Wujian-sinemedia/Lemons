#pragma once


namespace lemons::serializing
{
//==============================================================================
/**
    Implement this function and fromVar for any custom type to allow easily serialization using TreeReflector::add().
    This function must save the complete state of your object either directly to a juce::var or to a type that is implicitly convertable to juce::var (ie, float, int, juce::String, etc.).
    This function must do the exact opposite of what your implementation of fromVar for your type does.
    Usually what this boils down to is converting your object into a juce::String, which can then be implicitly converted to a juce::var.
    Several examples of implementations of this function can be found in lemons_serializing/Serializing/Specializations/VarSpecializations.cpp.
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
 
    @see fromVar, SerializableData, TreeReflector
 */
template < typename Type >
juce::var toVar (Type& object)
{
    return {object};
}


//==============================================================================
/**
    Implement this function and toVar for any custom type to allow easily serialization using TreeReflector::add().
    This function must load the complete state of your object from a juce::var.
    This function must do the exact opposite of what your implementation of toVar for your type does.
    Usually what this boils down to is creating a default-constructed instance of your object, then setting properties or relevant data members programmatically based on parsing the String form of the var.
    Several examples of implementations of this function can be found in lemons_serializing/Serializing/Specializations/VarSpecializations.cpp.
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
        MyCustomType fromVar (juce::var var) final
        {
            // implement your conversion logic here...
        }
    }
 
    // example usage:
    struct SomethingSerializable : lemons::SerializableData
    {
        void serialize (TreeReflector& ref)
        {
            // TreeReflector will internally call toVar to save your object and fromVar to load your object
            ref.add ("MyCustomData", data);
        }
 
        MyCustomType data;
    };
    @endcode
 
    @see toVar, SerializableData, TreeReflector
 */
template < typename Type >
Type fromVar (juce::var var)
{
    return {var};
}

}  // namespace lemons
