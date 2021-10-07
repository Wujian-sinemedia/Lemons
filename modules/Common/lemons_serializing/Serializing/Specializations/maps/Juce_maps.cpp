
namespace lemons::serializing
{

StringPairArrayInterface::StringPairArrayInterface (juce::StringPairArray& a)
: array(a)
{
    
}

int StringPairArrayInterface::getSize()
{
    return array.size();
}


String StringPairArrayInterface::getKey (int index)
{
    return array.getAllKeys()[index];
}

String StringPairArrayInterface::getValue (const String& key)
{
    return array.getValue (key, {});
}

void StringPairArrayInterface::setValue (const String& key, const String& value)
{
    array.set (key, value);
}


template <>
std::unique_ptr<MapInterfaceBase> getInterfaceForMap (juce::StringPairArray& array)
{
    return std::make_unique<StringPairArrayInterface> (array);
}

}
