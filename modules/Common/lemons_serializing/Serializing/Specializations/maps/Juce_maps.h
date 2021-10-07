#pragma once

namespace lemons::serializing
{

struct StringPairArrayInterface : MapInterface<String, String>
{
    StringPairArrayInterface (juce::StringPairArray& a);
    
    int getSize() final;
    
    String getKey (int index) final;
    
    String getValue (const String& key) final;
    
    void setValue (const String& key, const String& value) final;
    
private:
    juce::StringPairArray& array;
};

template <>
std::unique_ptr<MapInterfaceBase> getInterfaceForMap (juce::StringPairArray& array);

template <>
struct isMap< juce::StringPairArray > : std::true_type
{
};

}
