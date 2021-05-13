
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
    virtual void setValueFromString (juce::String string) = 0;
    virtual void setDefaultValueFromString (juce::String string) = 0;
    
    //
    
    void doAction()
    {
        actionableFunction();
    }
    
    void setFloatAction (std::function < void (float) > action)
    {
        floatAction = std::move(action);
        intAction  = nullptr;
        boolAction = nullptr;
        voidAction = nullptr;
    }
    
    void setIntAction (std::function < void (int) > action)
    {
        intAction = std::move(action);
        floatAction = nullptr;
        boolAction  = nullptr;
        voidAction  = nullptr;
    }
    
    void setBoolAction (std::function < void (bool) > action)
    {
        boolAction  = std::move(action);
        floatAction = nullptr;
        intAction   = nullptr;
        voidAction  = nullptr;
    }
    
    void setVoidAction (std::function < void () > action)
    {
        voidAction  = std::move(action);
        floatAction = nullptr;
        intAction   = nullptr;
        boolAction  = nullptr;
    }
    
    //
    
    void updateValueFromExternalSource()
    {
        valueUpdatingFunction();
    }
    
    std::function < void() > valueUpdatingFunction { [](){} };
    
    //
    
    virtual juce::ValueTree toValueTree() const = 0;
    
    //
    
    const int nodeID;
    
    const juce::String shortName;
    const juce::String longName;
    
    //==============================================================================
    
    struct Listener
    {
        virtual void propertyValueChanged (const juce::String& currentValueAsString) = 0;
        
        virtual void propertyDefaultValueChanged (const juce::String& currentValueAsString) { juce::ignoreUnused (currentValueAsString); }
    };
    
    //==============================================================================
    
    void addListener (Listener* l) { listeners.add (l); }
    void removeListener (Listener* l) { listeners.remove (l); }
    
protected:
    juce::ListenerList< Listener > listeners;
    
    std::function < void (float) >        floatAction;
    std::function < void (int) >          intAction;
    std::function < void (bool) >         boolAction;
    std::function < void (juce::String) > stringAction;
    
    std::function< void() > actionableFunction { [](){} };
};


//==============================================================================
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
    {
        Base::actionableFunction = [this]()
                                   {
                                       if (! Base::intAction)
                                           return;
                                       
                                       const auto value = getCurrentValue();
                                       
                                       if (value != lastActionedValue.load())
                                       {
                                           lastActionedValue.store (value);
                                           Base::intAction (value);
                                       }
                                   };
        
        Base::valueUpdatingFunction = [this]()
                                      {
                                          setValue (getNewValueFromExternalSource());
                                      };
        
        lastActionedValue.store (defaultValue.load());
    }
    
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
        if (currentValue.load() == newValue)
            return;
        
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
        setDefaultValue (getCurrentValue());
    }
    
    void resetToDefaultValue() override final
    {
        setValue (getDefaultValue());
    }
    
    void setValueFromString (juce::String string) override final
    {
        if (intFromString)
            setValue (intFromString (string));
        else
            setValue (backup_ValueFromString (string));
    }
    
    void setDefaultValueFromString (juce::String string) override final
    {
        if (intFromString)
            setDefaultValue (intFromString (string));
        else
            setDefaultValue (backup_ValueFromString (string));
    }
    
    //
    
    juce::ValueTree toValueTree() const override final
    {
        using namespace DefaultValueTreeIds;
        
        juce::ValueTree tree { NonParameterNode_Int };
        
        tree.setProperty (NonParameterName, Base::longName, nullptr);
        tree.setProperty (NonParameterValue, getCurrentValue(), nullptr);
        tree.setProperty (NonParameterDefaultValue, getDefaultValue(), nullptr);
        
        return tree;
    }
    
    //
    
    const int minValue, maxValue;
    
    std::function < juce::String (int, int) > stringFromInt;
    std::function < int (juce::String) > intFromString;
    
    std::function < int () > getNewValueFromExternalSource { [this](){ return getCurrentValue(); } };
    
    //
    
private:
    std::atomic<int> currentValue;
    std::atomic<int> defaultValue;
    
    int backup_ValueFromString (const juce::String& string)
    {
        return string.retainCharacters ("01234567890").getIntValue();
    }
    
    std::atomic<int> lastActionedValue;
};


//==============================================================================
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
    {
        Base::actionableFunction = [this]()
                                   {
                                       if (! Base::boolAction)
                                           return;
                                       
                                       const auto value = getCurrentValue();
                                       
                                       if (value != lastActionedValue.load())
                                       {
                                           lastActionedValue.store (value);
                                           Base::boolAction (value);
                                       }
                                   };
        
        Base::valueUpdatingFunction = [this]()
                                      {
                                          setValue (getNewValueFromExternalSource());
                                      };
        
        lastActionedValue.store (defaultValue.load());
    }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromBool)
            return stringFromBool (currentValue.load(), maximumLength);
        
        return {};
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromBool)
            return stringFromBool (defaultValue.load(), maximumLength);
        
        return {};
    }
    
    bool getCurrentValue() const { return currentValue.load(); }
    
    bool getDefaultValue() const { return defaultValue.load(); }
    
    //
    
    void setValue (bool newValue)
    {
        if (currentValue.load() == newValue)
            return;
        
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
        setDefaultValue (getCurrentValue());
    }
    
    void resetToDefaultValue() override final
    {
        setValue (getDefaultValue());
    }
    
    void setValueFromString (juce::String string) override final
    {
        if (boolFromString)
            setValue (boolFromString (string));
        else
            setValue (backup_ValueFromString (string));
    }
    
    void setDefaultValueFromString (juce::String string) override final
    {
        if (boolFromString)
            setDefaultValue (boolFromString (string));
        else
            setDefaultValue (backup_ValueFromString (string));
    }
    
    //
    
    juce::ValueTree toValueTree() const override final
    {
        using namespace DefaultValueTreeIds;
        
        juce::ValueTree tree { NonParameterNode_Bool };
        
        tree.setProperty (NonParameterName, Base::longName, nullptr);
        tree.setProperty (NonParameterValue, getCurrentValue(), nullptr);
        tree.setProperty (NonParameterDefaultValue, getDefaultValue(), nullptr);
        
        return tree;
    }
    
    //
    
    std::function < juce::String (bool, int) > stringFromBool;
    std::function < bool (juce::String) > boolFromString;
    
    std::function < bool () > getNewValueFromExternalSource { [this](){ return getCurrentValue(); } };
    
    //
    
private:
    std::atomic<bool> currentValue;
    std::atomic<bool> defaultValue;
    
    bool backup_ValueFromString (const juce::String& string)
    {
        if (string.containsWholeWordIgnoreCase ("Yes") || string.containsWholeWordIgnoreCase ("True"))
            return true;
        
        return false;
    }
    
    std::atomic<bool> lastActionedValue;
};


//==============================================================================
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
    {
        Base::actionableFunction = [this]()
                                   {
                                       if (! Base::floatAction)
                                           return;
                                       
                                       const auto value = getCurrentValue();
                                       
                                       if (value != lastActionedValue.load())
                                       {
                                           lastActionedValue.store (value);
                                           Base::floatAction (value);
                                       }
                                   };
        
        Base::valueUpdatingFunction = [this]()
                                      {
                                          setValue (getNewValueFromExternalSource());
                                      };
        
        lastActionedValue.store (defaultValue.load());
    }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromFloat)
            return stringFromFloat (currentValue.load(), maximumLength);
        
        return {};
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        if (stringFromFloat)
            return stringFromFloat (defaultValue.load(), maximumLength);
        
        return {};
    }
    
    float getCurrentValue() const { return currentValue.load(); }
    
    float getDefaultValue() const { return defaultValue.load(); }
    
    //
    
    void setValue (float newValue)
    {
        if (currentValue.load() == newValue)
            return;
        
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
        setDefaultValue (getCurrentValue());
    }
    
    void resetToDefaultValue() override final
    {
        setValue (getDefaultValue());
    }
    
    void setValueFromString (juce::String string) override final
    {
        if (floatFromString)
            setValue (floatFromString (string));
        else
            setValue (backup_ValueFromString (string));
    }
    
    void setDefaultValueFromString (juce::String string) override final
    {
        if (floatFromString)
            setDefaultValue (floatFromString (string));
        else
            setDefaultValue (backup_ValueFromString (string));
    }
    
    //
    
    juce::ValueTree toValueTree() const override final
    {
        using namespace DefaultValueTreeIds;
        
        juce::ValueTree tree { NonParameterNode_Float };
        
        tree.setProperty (NonParameterName, Base::longName, nullptr);
        tree.setProperty (NonParameterValue, getCurrentValue(), nullptr);
        tree.setProperty (NonParameterDefaultValue, getDefaultValue(), nullptr);
        
        return tree;
    }
    
    //
    
    const juce::NormalisableRange<float> range;
    
    std::function < juce::String (float, int) > stringFromFloat;
    std::function < float (juce::String) > floatFromString;
    
    std::function < float () > getNewValueFromExternalSource { [this](){ return getCurrentValue(); } };
    
    //
    
private:
    std::atomic<float> currentValue;
    std::atomic<float> defaultValue;
    
    float backup_ValueFromString (const juce::String& string)
    {
        return string.retainCharacters (".01234567890").getFloatValue();
    }
    
    std::atomic<float> lastActionedValue;
};


//==============================================================================
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
    {
        Base::actionableFunction = [this]()
                                   {
                                       if (! Base::stringAction)
                                           return;
                                       
                                       const auto value = getCurrentValue();
                                       
                                       if (value != lastActionedValue)
                                       {
                                           lastActionedValue = value;
                                           Base::stringAction (value);
                                       }
                                   };
        
        Base::valueUpdatingFunction = [this]()
                                      {
                                          setValue (getNewValueFromExternalSource());
                                      };
        
        lastActionedValue = defaultValue;
    }
    
    juce::String getCurrentValueAsString (int maximumLength = 100) const override final
    {
        return getCurrentValue().substring (0, maximumLength);
    }
    
    juce::String getDefaultValueAsString (int maximumLength = 100) const override final
    {
        return getDefaultValue().substring (0, maximumLength);
    }
    
    juce::String getCurrentValue() const
    {
        juce::ScopedLock l (lock);
        return currentValue;
    }
    
    juce::String getDefaultValue() const
    {
        juce::ScopedLock l (lock);
        return defaultValue;
    }
    
    //
    
    void setValue (const juce::String& newValue)
    {
        juce::ScopedLock sl (lock);
        
        if (currentValue == newValue)
            return;
        
        currentValue = newValue;
        const auto asString = getCurrentValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyValueChanged (asString); });
    }
    
    void setDefaultValue (juce::String newDefault)
    {
        juce::ScopedLock sl (lock);
        defaultValue = newDefault;
        const auto asString = getDefaultValueAsString();
        Base::listeners.call ([&asString] (Listener& l) { l.propertyDefaultValueChanged (asString); });
    }
    
    void setCurrentValueAsDefault() override final
    {
        setDefaultValue (getCurrentValue());
    }
    
    void resetToDefaultValue() override final
    {
        setValue (getDefaultValue());
    }
    
    void setValueFromString (juce::String string) override final
    {
        setValue (string);
    }
    
    void setDefaultValueFromString (juce::String string) override final
    {
        setDefaultValue (string);
    }
    
    //
    
    juce::ValueTree toValueTree() const override final
    {
        using namespace DefaultValueTreeIds;
        
        juce::ValueTree tree { NonParameterNode_String };
        
        tree.setProperty (NonParameterName, Base::longName, nullptr);
        tree.setProperty (NonParameterValue, getCurrentValue(), nullptr);
        tree.setProperty (NonParameterDefaultValue, getDefaultValue(), nullptr);
        
        return tree;
    }
    
    //
    
    std::function < juce::String () > getNewValueFromExternalSource { [this](){ return getCurrentValue(); } };
    
private:
    juce::String currentValue;
    juce::String defaultValue;
    
    juce::CriticalSection lock;
    
    juce::String lastActionedValue;
};


}  // namespace

