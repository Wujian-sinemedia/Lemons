
/** @ingroup SerializableData
 *  @{
 */
#pragma once

#include <type_traits>
#include <memory>

namespace lemons::serializing
{
struct MapInterfaceBase
{
    virtual ~MapInterfaceBase() = default;
};

template < typename KeyType, typename ValueType >
struct MapInterface : MapInterfaceBase
{
    using key_type   = KeyType;
    using value_type = ValueType;

    virtual int       getSize()                     = 0;
    virtual KeyType   getKey (int index)            = 0;
    virtual ValueType getValue (const KeyType& key) = 0;

    virtual void setValue (const KeyType& key, const ValueType& value) = 0;
};


template < typename MapType >
std::unique_ptr< MapInterfaceBase > getInterfaceForMap (MapType&);


/** Template struct that indicates to TreeReflector whether a given type is a map.
    @see TreeReflector
 */
template < typename Type >
struct isMap : std::false_type
{
};

}  // namespace lemons::serializing

/** @}*/
