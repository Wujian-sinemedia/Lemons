
namespace bav
{

// this struct holds the data for a ValueTree property node that will not be represented by an actual parameter object
struct NonParamValueTreeNode
{
    NonParamValueTreeNode (NonAutomatableParameterID id,
                           const juce::String& nameShort,
                           const juce::String& nameVerbose)
    : nodeID (id),
    shortName (TRANS (nameShort)),
    longName (TRANS (nameVerbose))
    { }
    
    const NonAutomatableParameterID nodeID;
    
    const juce::String shortName;
    const juce::String longName;
};


//==============================================================================


struct IntValueTreeNode  :  NonParamValueTreeNode
{
    IntValueTreeNode (NonAutomatableParameterID id,
                      const juce::String& nameShort,
                      const juce::String& nameVerbose,
                      int min, int max, int defaultVal,
                      std::function < juce::String (int, int) > stringFromIntFunc = nullptr,
                      std::function < int (juce::String) > intFromStringFunc = nullptr)
    : NonParamValueTreeNode (id, nameShort, nameVerbose),
    minValue (min), maxValue(max), defaultValue (defaultVal),
    stringFromInt (std::move (stringFromIntFunc)),
    intFromString (std::move (intFromStringFunc))
    { }
    
    const int minValue, maxValue;
    
    int defaultValue;
    
    std::atomic<int> currentValue;
    
    std::function < juce::String (int, int) > stringFromInt;
    std::function < int (juce::String) > intFromString;
};


//==============================================================================


struct BoolValueTreeNode   :  NonParamValueTreeNode
{
    BoolValueTreeNode (NonAutomatableParameterID id,
                       const juce::String& nameShort,
                       const juce::String& nameVerbose,
                       bool defaultVal,
                       std::function < juce::String (bool, int) > stringFromBoolFunc = nullptr,
                       std::function < int (juce::String) > boolFromStringFunc = nullptr)
    : NonParamValueTreeNode (id, nameShort, nameVerbose),
    defaultValue (defaultVal),
    stringFromBool (std::move (stringFromBoolFunc)),
    boolFromString (std::move (boolFromStringFunc))
    { }
    
    bool defaultValue;
    
    std::atomic<bool> currentValue;
    
    std::function < juce::String (bool, int) > stringFromBool;
    std::function < bool (juce::String) > boolFromString;
};


//==============================================================================


struct FloatValueTreeNode  :  NonParamValueTreeNode
{
    FloatValueTreeNode (NonAutomatableParameterID id,
                        const juce::String& nameShort,
                        const juce::String& nameVerbose,
                        juce::NormalisableRange<float> normRange,
                        float defaultVal,
                        std::function < juce::String (float, int) > stringFromFloatFunc = nullptr,
                        std::function < float (juce::String) > floatFromStringFunc = nullptr)
    : NonParamValueTreeNode (id, nameShort, nameVerbose),
    range (normRange), defaultValue (defaultVal),
    stringFromFloat (std::move (stringFromFloatFunc)),
    floatFromString (std::move (floatFromStringFunc))
    { }
    
    const juce::NormalisableRange<float> range;
    
    float defaultValue;
    
    std::atomic<float> currentValue;
    
    std::function < juce::String (float, int) > stringFromFloat;
    std::function < float (juce::String) > floatFromString;
};


//==============================================================================


struct StringValueTreeNode :  NonParamValueTreeNode
{
    StringValueTreeNode (NonAutomatableParameterID id,
                         const juce::String& nameShort,
                         const juce::String& nameVerbose,
                         const juce::String& defaultVal)
    : NonParamValueTreeNode (id, nameShort, nameVerbose),
    defaultValue (defaultVal)
    { }
    
    juce::String defaultValue;
};


//==============================================================================


struct NonParamValueTreeNodeGroup
{
    NonParamValueTreeNodeGroup (const juce::String& groupName)
    : name (TRANS (groupName))
    { }
    
    const NonParamValueTreeNode* const* begin() const noexcept
    {
        return const_cast<const NonParamValueTreeNode**> (children.begin());
    }
    
    const NonParamValueTreeNode* const* end() const noexcept
    {
        return const_cast<const NonParamValueTreeNode**> (children.end());
    }
    
    
    void addChild (NonParamValueTreeNode* node) { children.add (node); }
    
    
    const juce::String name;
    
private:
    
    juce::OwnedArray <NonParamValueTreeNode> children;
};


}  // namespace
