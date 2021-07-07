
#pragma once

#include "TreeReflector.h"

namespace bav
{
struct SerializableData
{
    SerializableData (juce::Identifier identifier = "Data");
    virtual ~SerializableData() = default;

    SerializableData& operator= (SerializableData& other);

    ValueTree serialize();
    ValueTree serialize (juce::Identifier treeID);

    void deserialize (const ValueTree& tree);

    const juce::Identifier& getDataIdentifier() const { return dataIdentifier; }

private:
    virtual void serialize (TreeReflector& ref) = 0;

    void setTree (const ValueTree& newTree);

    const juce::Identifier dataIdentifier;
};


}  // namespace bav
