#pragma once

namespace lemons::serializing
{
/** Template struct that indicates to TreeReflector whether a given type is a map.
    @see TreeReflector
 */
template < typename Type >
struct isMap : std::false_type
{
};

}  // namespace lemons::serializing
