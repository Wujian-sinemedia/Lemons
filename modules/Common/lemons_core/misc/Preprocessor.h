/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


#pragma once


/** Use this macro inside a class template to place inheritance constraints on a template argument.
    For example:
    @code
    template < class ParameterType, LEMONS_MUST_INHERIT_FROM(ParameterType, juce::AudioProcessorParameter) >
    class SomeClass { };
    @endcode
    With the above code, attempting to instantiate a specialization of SomeClass where the template argument is not a type that inherits from juce::AudioProcessorParameter will throw a compile-time error. \n
    Note that both arguments to this macro must be fully specialized types.
 */
#define LEMONS_MUST_INHERIT_FROM(classToTest, requiredBaseClass) \
  std::enable_if_t<std::is_base_of<requiredBaseClass, classToTest>::value>* = nullptr
