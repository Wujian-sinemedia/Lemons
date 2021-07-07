#pragma once

namespace bav
{
template < class Test, template < typename... > class Ref >
struct is_specialization : std::false_type
{
};

template < template < typename... > class Ref, typename... Args >
struct is_specialization< Ref< Args... >, Ref > : std::true_type
{
};

template < class Type >
struct is_vector : is_specialization< Type, std::vector >
{
};

//template<class Type>
//struct is_juce_array : is_specialization<Type, juce::Array> {};

template < class Type >
struct is_juce_owned_array : is_specialization< Type, juce::OwnedArray >
{
};


template < class Type >
constexpr bool is_container()
{
    if constexpr (std::is_array_v< Type > || is_vector< Type >() || is_juce_owned_array< Type >())
        return true;

    return false;
}

}  // namespace bav
