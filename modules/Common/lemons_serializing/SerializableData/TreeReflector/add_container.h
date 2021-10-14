#pragma once

namespace lemons
{

template < class ContainerType >
void TreeReflector::addContainer (ContainerType& container)
{
    const auto singlePropertyName = [str = tree.getType().toString()]() -> String
    {
        if (str.endsWith ("s"))
            str.dropLastCharacters (1);
        
        return str + "_";
    }();
    
    auto makePropertyNameForContainerElement = [&] (int index) -> String
    {
        return singlePropertyName + String (index);
    };
    
    if (isLoading())
    {
        using namespace serializing;
        
        const auto numElements = [&]() -> int
        {
            using ElementType = typename std::decay_t< decltype (container.begin()) >;
            
            constexpr auto elementsAreContainers = isContainer< ElementType >();
            
            const auto total = elementsAreContainers ? tree.getNumChildren() : tree.getNumProperties();
            
            juce::Array< String > names;
            
            for (int i = 1; i <= total; ++i)
                names.add (makePropertyNameForContainerElement (i));
            
            auto actualNum = total;
            
            for (int i = 0; i < total; ++i)
            {
                const auto test = elementsAreContainers ? tree.getChild (i).getType() : tree.getPropertyName (i);
                
                if (! names.contains (test.toString()))
                    --actualNum;
            }
            
            return actualNum;
        }();
        
        getInterfaceForContainer (container)->resize (numElements);
    }
    
    for (int index = 1; auto& element : container)
        add (makePropertyNameForContainerElement (index++),
             element);
}

}
