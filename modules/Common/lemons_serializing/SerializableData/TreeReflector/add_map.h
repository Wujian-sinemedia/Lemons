#pragma once

namespace lemons
{

template < class MapType >
void TreeReflector::addMap (MapType& map)
{
    using namespace serializing;
    
    auto interface = getInterfaceForMap (map);
    
    if (isLoading())
    {
        using InterfaceType = typename decltype (interface)::element_type;
        using KeyType       = typename InterfaceType::key_type;
        using ValueType     = typename InterfaceType::value_type;
        
        for (int i = 0; i < tree.getNumProperties(); ++i)
        {
            const auto name = tree.getPropertyName (i);
            
            interface->setValue (fromVar< KeyType > (name),
                                 fromVar< ValueType > (tree.getProperty (name)));
        }
    }
    else
    {
        for (int i = 0; i < interface->getSize(); ++i)
        {
            const auto key = interface->getKey (i);
            
            tree.setProperty (toVar (key),
                              toVar (interface->getValue (key)),
                              nullptr);
        }
    }
}

}
