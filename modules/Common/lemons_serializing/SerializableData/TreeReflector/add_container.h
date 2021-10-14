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

    auto makePropertyNameForContainerElement = [singlePropertyName] (int index) -> String
    {
        return singlePropertyName + String (index);
    };

    if (isLoading())  // resize the container
    {
        using namespace serializing;

        const auto numElements = [&]() -> int
        {
            constexpr auto elementsAreContainers = isContainer< std::decay_t< decltype (container.begin()) > >();

            const auto total = elementsAreContainers ? tree.getNumChildren() : tree.getNumProperties();

            juce::StringArray names;

            for (int i = 1; i <= total; ++i)
                names.add (makePropertyNameForContainerElement (i));

            jassert (names.size() == total);

            auto actualNum = total;

            for (int i = 0; i < total; ++i)
            {
                if (const auto test = elementsAreContainers ? tree.getChild (i).getType() : tree.getPropertyName (i);
                    ! names.contains (test.toString()))
                {
                    --actualNum;
                }
            }

            return actualNum;
        }();

        getInterfaceForContainer (container)->resize (numElements);
    }

    for (int   index = 1;
         auto& element : container)
        add (makePropertyNameForContainerElement (index++), element);
}

}  // namespace lemons
