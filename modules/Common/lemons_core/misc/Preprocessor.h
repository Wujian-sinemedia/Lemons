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

#include <type_traits>
#include <vector>
#include <list>

/** @defgroup lemons_core
    Core building blocks.
 */

/** @ingroup lemons_core
    Use this macro inside a template declaration to place inheritance constraints on a template argument.
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

namespace lemons
{

/** @ingroup lemons_core
    Utility struct that evaluates to std::true_type if the given class specializes the given template, and false otherwise.
    For example:
    @code
    is_specialization< std::vector<int>, std::vector >()
    @endcode
    evaluates to a compile-time true value, and
    @code
    is_specialization< std::vector<int>, std::list >()
    @endcode
    evaluates to a compile-time false value.
 */
template <class T, template <class...> class Template>
struct is_specialization : std::false_type
{
};

template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type
{
};

static_assert (is_specialization<std::vector<int>, std::vector>(), "is_specialization test");
static_assert (! is_specialization<std::vector<int>, std::list>(), "is_specialization test");

}  // namespace lemons

/** @ingroup lemons_core
    Use this macro inside a template declaration to ensure that classToTest is a specialization of the required template.
    For example:
    @code
    template < class VectorType, LEMONS_MUST_BE_SPECIALIZATION(VectorType, std::vector) >
    class SomeClass { };
    @endcode
    With the above code, instantiating @code SomeClass< std::vector<int> > @endcode will succeed, and attempting to instantiate @code SomeClass< std::list<int> >; @endcode will throw a compile-time error. \n
    Note that the first argument to this macro should be a fully-specialized type, and the second argument to this macro must be an unspecialized template!
 */
#define LEMONS_MUST_BE_SPECIALIZATION(classToTest, requiredTemplate) \
  std::enable_if_t<lemons::is_specialization<classToTest, requiredTemplate>::value>* = nullptr
