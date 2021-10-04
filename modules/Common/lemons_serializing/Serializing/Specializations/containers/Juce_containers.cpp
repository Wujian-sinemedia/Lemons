
namespace lemons::serializing
{
StringArrayInterface::StringArrayInterface (juce::StringArray& array)
    : LambdaContainerInterface< juce::StringArray > (array, [] (juce::StringArray& a, int newSize)
                                                     {
                                                         a.clear();
                                                         a.ensureStorageAllocated (newSize);
                                                     })
{
}

std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::StringArray& container)
{
    return std::make_unique< StringArrayInterface > (container);
}

}  // namespace lemons::serializing
