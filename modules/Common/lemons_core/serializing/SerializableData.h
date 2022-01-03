#pragma once

#include <juce_data_structures/juce_data_structures.h>

namespace lemons::serializing
{

using juce::String;
using juce::ValueTree;


struct SerializableData
{
    virtual ~SerializableData() = 0;
    
    [[nodiscard]] virtual String getValueTreeType() const = 0;
    
    [[nodiscard]] virtual ValueTree saveToValueTree() const = 0;
    
    virtual void restoreFromValueTree (const ValueTree& tree) = 0;
};

}
