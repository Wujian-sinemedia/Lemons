
#pragma once

#include "Reflector.h"

namespace bav
{
struct SerializableData
{
    SerializableData (juce::Identifier identifier = "Data");
    virtual ~SerializableData() = default;

    SerializableData& operator= (SerializableData& other);

    ValueTree serialize();
    void deserialize (const ValueTree& tree);

    const juce::Identifier& getDataIdentifier() const { return dataIdentifier; }

private:
    friend struct TreeReflector;
    
    virtual void serialize (TreeReflector& ref) = 0;
    
    ValueTree saveToTree (const String& treeName);
    void restoreFromTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
};


}  // namespace bav
