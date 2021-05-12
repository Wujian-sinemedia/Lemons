
namespace bav
{

// this struct holds the data for a ValueTree property node that will not be represented by an actual parameter object
struct NonParamValueTreeNode
{
    NonParamValueTreeNode (int id,
                           const juce::String& nameShort,
                           const juce::String& nameVerbose)
      : nodeID (id),
        shortName (TRANS (nameShort)),
        longName (TRANS (nameVerbose))
    { }
    
    virtual ~NonParamValueTreeNode() = default;
    
    //
    
    virtual juce::String getCurrentValueAsString (int maximumLength = 100) const = 0;
    
    virtual juce::String getDefaultValueAsString (int maximumLength = 100) const = 0;
    
    //
    
    virtual void setCurrentValueAsDefault() = 0;
    
    virtual void resetToDefaultValue() = 0;
    
    //
    
    void doAction()
    {
        if (actionableFunction)
            actionableFunction (getCurrentValueAsString());
    }
    
    std::function< void (juce::String currentValueAsString) > actionableFunction = nullptr;
    
    //
    
    const int nodeID;
    
    const juce::String shortName;
    const juce::String longName;
    
    //==============================================================================
    
    struct Listener
    {
        virtual void propertyValueChanged (const juce::String& currentValueAsString) = 0;
        
        virtual void propertyDefaultValueChanged (const juce::String& currentValueAsString);
    };
    
    //==============================================================================
    
    void addListener (Listener* l) { listeners.add (l); }
    
    void removeListener (Listener* l) { listeners.remove (l); }
    
    
protected:
    juce::ListenerList< Listener > listeners;
};


//==============================================================================


struct IntValueTreeNode  :  NonParamValueTreeNode
{
    using Base = NonParamValueTreeNode;
    using Listener = NonParamValueTreeNode::Listener;
    
    IntValueTreeNode (int id,
                      const juce::String& nameShort,
                      const juce::String& nameVerbose,
                      int min, int max, int defaultVal,
                      std::function < juce::String (int, int) > stringFromIntFunc = nullptr,
                      std::function < int (juce::String) > intFromStringFunc = nullptr)
      : NonParamValueTreeNode (id, nameShort, nameVerbose),
        minValue (min), maxValue(max),
        stringFromInt (std::move (stringFromIntFunc)),
        intFromString (std::move (intFromStringFunc)),
        currentValue (defaultVal), defaultValue (defaultVal)
    { }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromInt)
            return stringFromInt (currentValue.load(), maximumLength);
        
        return {};
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromInt)
            return stringFromInt (defaultValue.load(), maximumLength);
        
        return {};
    }
    
    int getCurrentValue() const { return currentValue.load(); }
    
    int getDefaultValue() const { return defaultValue.load(); }
    
    //
    
    void setValue (int newValue)
    {
        currentValue.store (newValue);
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    void setDefaultValue (int newDefault)
    {
        defaultValue.store (newDefault);
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void setCurrentValueAsDefault() override final
    {
        defaultValue.store (currentValue.load());
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void resetToDefaultValue() override final
    {
        currentValue.store (defaultValue.load());
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    //
    
    const int minValue, maxValue;
    
    std::function < juce::String (int, int) > stringFromInt;
    std::function < int (juce::String) > intFromString;
    
    //
    
private:
    std::atomic<int> currentValue;
    std::atomic<int> defaultValue;
};


//==============================================================================


struct BoolValueTreeNode   :  NonParamValueTreeNode
{
    using Base = NonParamValueTreeNode;
    using Listener = NonParamValueTreeNode::Listener;
    
    BoolValueTreeNode (int id,
                       const juce::String& nameShort,
                       const juce::String& nameVerbose,
                       bool defaultVal,
                       std::function < juce::String (bool, int) > stringFromBoolFunc = nullptr,
                       std::function < int (juce::String) > boolFromStringFunc = nullptr)
      : NonParamValueTreeNode (id, nameShort, nameVerbose),
        stringFromBool (std::move (stringFromBoolFunc)),
        boolFromString (std::move (boolFromStringFunc)),
        currentValue (defaultVal), defaultValue (defaultVal)
    { }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromBool)
            return stringFromBool (currentValue.load(), maximumLength);
        
        return {};
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromBool)
            return stringFromBool (defaultValue, maximumLength);
        
        return {};
    }
    
    bool getCurrentValue() const { return currentValue.load(); }
    
    bool getDefaultValue() const { return defaultValue.load(); }
    
    //
    
    void setValue (bool newValue)
    {
        currentValue.store (newValue);
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    void setDefaultValue (bool newDefault)
    {
        defaultValue.store (newDefault);
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void setCurrentValueAsDefault() override final
    {
        defaultValue.store (currentValue.load());
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void resetToDefaultValue() override final
    {
        currentValue.store (defaultValue.load());
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    //
    
    std::function < juce::String (bool, int) > stringFromBool;
    std::function < bool (juce::String) > boolFromString;
    
    //
    
private:
    std::atomic<bool> currentValue;
    std::atomic<bool> defaultValue;
};


//==============================================================================


struct FloatValueTreeNode  :  NonParamValueTreeNode
{
    using Base = NonParamValueTreeNode;
    using Listener = NonParamValueTreeNode::Listener;
    
    FloatValueTreeNode (int id,
                        const juce::String& nameShort,
                        const juce::String& nameVerbose,
                        juce::NormalisableRange<float> normRange,
                        float defaultVal,
                        std::function < juce::String (float, int) > stringFromFloatFunc = nullptr,
                        std::function < float (juce::String) > floatFromStringFunc = nullptr)
      : NonParamValueTreeNode (id, nameShort, nameVerbose),
        range (normRange),
        stringFromFloat (std::move (stringFromFloatFunc)),
        floatFromString (std::move (floatFromStringFunc)),
        currentValue (defaultVal), defaultValue (defaultVal)
    { }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromFloat)
            return stringFromFloat (currentValue.load(), maximumLength);
        
        return {};
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromFloat)
            return stringFromFloat (defaultValue, maximumLength);
        
        return {};
    }
    
    float getCurrentValue() const { return currentValue.load(); }
    
    float getDefaultValue() const { return defaultValue.load(); }
    
    //
    
    void setValue (float newValue)
    {
        currentValue.store (newValue);
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    void setDefaultValue (float newDefault)
    {
        defaultValue.store (newDefault);
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void setCurrentValueAsDefault() override final
    {
        defaultValue.store (currentValue.load());
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void resetToDefaultValue() override final
    {
        currentValue.store (defaultValue.load());
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    //
    
    const juce::NormalisableRange<float> range;
    
    std::function < juce::String (float, int) > stringFromFloat;
    std::function < float (juce::String) > floatFromString;
    
    //
    
private:
    std::atomic<float> currentValue;
    std::atomic<float> defaultValue;
};


//==============================================================================


struct StringValueTreeNode :  NonParamValueTreeNode
{
    using Base = NonParamValueTreeNode;
    using Listener = NonParamValueTreeNode::Listener;
    
    StringValueTreeNode (int id,
                         const juce::String& nameShort,
                         const juce::String& nameVerbose,
                         const juce::String& defaultVal)
      : NonParamValueTreeNode (id, nameShort, nameVerbose),
        currentValue (defaultVal), defaultValue (defaultVal)
    { }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        return currentValue.substring (0, maximumLength);
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        return defaultValue.substring (0, maximumLength);
    }
    
    juce::String getCurrentValue() const { return currentValue; }
    
    juce::String getDefaultValue() const { return defaultValue; }
    
    //
    
    void setValue (juce::String newValue)
    {
        currentValue = newValue;
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    void setDefaultValue (juce::String newDefault)
    {
        defaultValue = newDefault;
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void setCurrentValueAsDefault() override final
    {
        defaultValue = currentValue;
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void resetToDefaultValue() override final
    {
        currentValue = defaultValue;
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    //
    
private:
    juce::String currentValue;
    juce::String defaultValue;
};


//==============================================================================


struct NonParamValueTreeNodeGroup
{
    //==============================================================================
    // A node of a NonParamValueTreeNodeGroup. This can contain either another NonParamValueTreeNodeGroup or a kind of NonParamValueTreeNode
    
    struct NonParamValueTreeNodeGroupNode
    {
        NonParamValueTreeNodeGroupNode (NonParamValueTreeNodeGroupNode&& other)
          : group (std::move (other.group)), intNode (std::move (other.intNode)), boolNode (std::move (other.boolNode)),
            floatNode (std::move (other.floatNode)), stringNode (std::move (other.stringNode))
        { }
        
        ~NonParamValueTreeNodeGroupNode() = default;
        
        //
        
        NonParamValueTreeNodeGroup* getGroup() const { return group.get(); }
        IntValueTreeNode*    getIntNode()    const { return intNode.get(); }
        BoolValueTreeNode*   getBoolNode()   const { return boolNode.get(); }
        FloatValueTreeNode*  getFloatNode()  const { return floatNode.get(); }
        StringValueTreeNode* getStringNode() const { return stringNode.get(); }
        
    private:
        NonParamValueTreeNodeGroupNode (std::unique_ptr<NonParamValueTreeNodeGroup> group)
          : group (std::move (group))
        { }
        
        NonParamValueTreeNodeGroupNode (std::unique_ptr<IntValueTreeNode> intnode)
          : intNode (std::move (intnode))
        { }
        
        NonParamValueTreeNodeGroupNode (std::unique_ptr<BoolValueTreeNode> boolnode)
          : boolNode (std::move (boolnode))
        { }
        
        NonParamValueTreeNodeGroupNode (std::unique_ptr<FloatValueTreeNode> floatnode)
          : floatNode (std::move (floatnode))
        { }
        
        NonParamValueTreeNodeGroupNode (std::unique_ptr<StringValueTreeNode> stringnode)
          : stringNode (std::move (stringnode))
        { }
        
        //
        
        std::unique_ptr<NonParamValueTreeNodeGroup> group;
        std::unique_ptr<IntValueTreeNode> intNode;
        std::unique_ptr<BoolValueTreeNode> boolNode;
        std::unique_ptr<FloatValueTreeNode> floatNode;
        std::unique_ptr<StringValueTreeNode> stringNode;
        
        friend struct NonParamValueTreeNodeGroup;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonParamValueTreeNodeGroupNode)
    };
    
    //==============================================================================
    
    NonParamValueTreeNodeGroup (const juce::String& groupName)
        : name (TRANS (groupName))
    { }
    
    template <typename NodeOrSubgroup>
    NonParamValueTreeNodeGroup (const juce::String& groupName,
                                std::unique_ptr<NodeOrSubgroup> child)
        : NonParamValueTreeNodeGroup (groupName)
    {
        addChild (std::move (child));
    }

    template <typename NodeOrSubgroup, typename... Args>
    NonParamValueTreeNodeGroup (const juce::String& groupName,
                                std::unique_ptr<NodeOrSubgroup> firstChild, Args&&... remainingChildren)
        : NonParamValueTreeNodeGroup (groupName, std::move (firstChild))
    {
        addChild (std::forward<Args> (remainingChildren)...);
    }
    
    //
    
    const NonParamValueTreeNodeGroupNode* const* begin() const noexcept
    {
        return const_cast<const NonParamValueTreeNodeGroupNode**> (children.begin());
    }
    
    const NonParamValueTreeNodeGroupNode* const* end() const noexcept
    {
        return const_cast<const NonParamValueTreeNodeGroupNode**> (children.end());
    }
    
    
    template <typename NodeOrSubgroup>
    void addChild (std::unique_ptr<NodeOrSubgroup> child)
    {
        /* If you get a compiler error here, then you are attempting to add a child that is neither a NonParamValueTreeNodeGroup or a node type. */
        append (std::move (child));
    }
    
    template <typename NodeOrSubgroup, typename... Args>
    void addChild (std::unique_ptr<NodeOrSubgroup> firstChild, Args&&... remainingChildren)
    {
        addChild (std::move (firstChild));
        addChild (std::forward<Args> (remainingChildren)...);
    }
    
    
    const juce::String name;
    
    //
    
private:
    void append (std::unique_ptr<NonParamValueTreeNodeGroup> group)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (group)));
    }
    
    void append (std::unique_ptr<IntValueTreeNode> node)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (node)));
    }
    
    void append (std::unique_ptr<BoolValueTreeNode> node)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (node)));
    }
    
    void append (std::unique_ptr<FloatValueTreeNode> node)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (node)));
    }
    
    void append (std::unique_ptr<StringValueTreeNode> node)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (node)));
    }
    
    //
    
    juce::OwnedArray <NonParamValueTreeNodeGroupNode> children;
};


}  // namespace

