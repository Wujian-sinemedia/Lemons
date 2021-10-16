
/** @ingroup misc
 *  @{
 */
#pragma once

/** Expands to true if this module was cross-compiled for either iOS or Android. */
#define LEMONS_MOBILE (JUCE_IOS || JUCE_ANDROID)


/** Use this macro inside a class template to place inheritance constraints on a template argument.
    For example:
    @code
    template < class SerializableType, LEMONS_MUST_INHERIT_FROM(SerializableType, lemons::SerializableData) >
    class SomeClass { };
    @endcode
    With the above code, attempting to instantiate a specialization of SomeClass where the template argument is not a type that inherits from SerializableData will throw a compile-time error. \n
    Note that both arguments to this macro must be fully specialized types.
 */
#define LEMONS_MUST_INHERIT_FROM(classToTest, requiredBaseClass) \
    std::enable_if_t< std::is_base_of< requiredBaseClass, classToTest >::value >* = nullptr

/** @}*/
