

namespace bav
{
    
    class Parameter     :   private juce::AudioProcessorParameter::Listener
    {
        using RangedParam = juce::RangedAudioParameter;
        
    public:
        Parameter (int key, RangedParam* p, float defaultValue,
                   juce::String paramNameShort,
                   juce::String paramNameVerbose)
              : parameterNameShort   (TRANS (paramNameShort)),
                parameterNameVerbose (TRANS (paramNameVerbose)),
                parameterNameVerboseNoSpaces (paramNameVerbose.trim().replaceCharacters (" ", "_")),
                currentDefault (defaultValue), rap (p), keyID (key)
        {
            jassert (rap != nullptr);
            lastActionedValue.store (defaultValue);
            rap->addListener (this);
            changing.store (false);
        }
        
        virtual ~Parameter() override
        {
            rap->removeListener (this);
        }
        
        //==============================================================================
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        float getDenormalizedDefault() const { return denormalize (currentDefault.load()); }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            currentDefault.store (getCurrentNormalizedValue());
        }
        
        void setNormalizedDefault (float value)
        {
            jassert (value >= 0.0f && value <= 1.0f);
            
            if (currentDefault.load() == value)
                return;
            
            currentDefault.store (value);
            
            if (onDefaultChange)
                bav::callOnMessageThread<float> (onDefaultChange, value);
        }
        
        void setDefault (float value)
        {
            setNormalizedDefault (normalize (value));
        }
        
        void setDefault (int value)
        {
            setNormalizedDefault (normalize (static_cast<float> (value)));
        }
        
        void setDefault (bool value)
        {
            const auto def = value ? 1.0f : 0.0f;
            setNormalizedDefault (def);
        }
        
        // resets the parameter to its currently stored default
        void resetToDefault() { rap->setValueNotifyingHost (currentDefault.load()); }
        
        //==============================================================================
        
        float getCurrentNormalizedValue()   const { return rap->getValue(); }
        float getCurrentDenormalizedValue() const { return rap->convertFrom0to1 (rap->getValue()); }
        
        // these functions return the current denormalized value as the desired type literal
        float getFloatValue() const { return getCurrentDenormalizedValue(); }
        int   getIntValue()   const { return juce::roundToInt (getCurrentDenormalizedValue()); }
        bool  getBoolValue()  const { return getCurrentNormalizedValue() >= 0.5f; }
        
        //==============================================================================
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        virtual float normalize (const float input) const { return rap->convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        virtual float denormalize (const float input) const { return rap->convertFrom0to1(input); }
        
        //==============================================================================
        
        // exposes the underlying juce::RangedAudioParameter
        RangedParam* orig() const noexcept { return rap; }
        
        int key() const noexcept { return keyID; }
        
        //==============================================================================
        /* You can specify an action of a certain type, to be performed synchronously when you call doAction(), only if the parameter calue has changed since the last call to doAction().
           You can only specify one type of action at a time. Each type will be called with the denormalized current parameter value in its natural range -- so, float actions will recieve denormalized values, int actions will recieve denormalized floats rounded to integers, and bool actions will recieve boolean literals. You can also specify a void action, that will be called with no arguments.
         */
        
        void doAction()
        {
            const auto value = getCurrentNormalizedValue();
            
            if (value != lastActionedValue.load())
            {
                lastActionedValue.store (value);
                
                if (floatAction)
                    floatAction (getFloatValue());
                else if (intAction)
                    intAction (getIntValue());
                else if (boolAction)
                    boolAction (getBoolValue());
                else if (voidAction)
                    voidAction();
            }
        }
        
        void setFloatAction (std::function < void (float) > action)
        {
            floatAction = std::move(action);
            intAction   = nullptr;
            boolAction  = nullptr;
            voidAction  = nullptr;
        }
        
        void setIntAction (std::function < void (int) > action)
        {
            intAction   = std::move(action);
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
        
        //==============================================================================
        // if defined, these functions will be called on the message thread when this parameter changes ("somewhat synchronously")
        std::function< void (float) > onParameterChange; // this gets the denormalized new float value
        std::function< void (float) > onDefaultChange;
        std::function< void (bool)  > onGestureStateChange;
        
        //==============================================================================
        
        void parameterValueChanged (int, float) override final
        {
            if (onParameterChange)
                bav::callOnMessageThread<float> (onParameterChange, getCurrentDenormalizedValue());
        }
        
        void parameterGestureChanged (int, bool gestureIsStarting) override final
        {
            changing.store (gestureIsStarting);
            if (onGestureStateChange)
                bav::callOnMessageThread<bool> (onGestureStateChange, gestureIsStarting);
        }
        
        //==============================================================================
        
        virtual juce::ValueTree toValueTree() const = 0;
        
        //==============================================================================
        
        bool isChanging() const { return changing.load(); }
        
        const juce::String parameterNameShort;
        const juce::String parameterNameVerbose;
        const juce::String parameterNameVerboseNoSpaces;
        
        //==============================================================================
        
    private:
        RangedParam* const rap;
        const int keyID;
        
        std::atomic<float> currentDefault;
        std::atomic<bool> changing;
        
        //==============================================================================
        
        std::function < void (float) > floatAction;
        std::function < void (int) >   intAction;
        std::function < void (bool) >  boolAction;
        std::function < void () >      voidAction;
        
        std::atomic<float> lastActionedValue;
    };
    
    
//==============================================================================

    
    class FloatParameter  :     public juce::AudioParameterFloat,
                                public bav::Parameter
    {
        using AudioParameterFloat = juce::AudioParameterFloat;
        
    public:
        FloatParameter (int key,
                        juce::String paramNameShort, juce::String paramNameVerbose,
                        juce::NormalisableRange<float> nRange, float defaultVal, juce::String parameterLabel = juce::String(),
                        juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter,
                        std::function<juce::String(float value, int maximumStringLength)> stringFromValue = nullptr,
                        std::function<float(const juce::String& text)> valueFromString = nullptr)
             :  AudioParameterFloat (paramNameVerbose, TRANS (paramNameVerbose), nRange, defaultVal,
                                     parameterLabel, parameterCategory, stringFromValue, valueFromString),
                Parameter (key, this, nRange.convertTo0to1 (defaultVal), paramNameShort, paramNameVerbose),
                floatToString (stringFromValue), stringToFloat(valueFromString)
        { }
        
        float getDefault() const { return Parameter::getDenormalizedDefault(); }
        
        std::function< juce::String (float, int) > floatToString;
        std::function< float (const juce::String&) > stringToFloat;
        
        juce::ValueTree toValueTree() const override
        {
            using namespace DefaultValueTreeIds;
            
            juce::ValueTree tree { Parameter_Float };
            
            tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
            tree.setProperty (ParameterValue, AudioParameterFloat::get(), nullptr);
            tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
            tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);
            
            return tree;
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatParameter)
    };
    

//==============================================================================


    class IntParameter    :     public juce::AudioParameterInt,
                                public bav::Parameter
    {
        using AudioParameterInt = juce::AudioParameterInt;
        
    public:
        IntParameter (int key,
                      juce::String paramNameShort, juce::String paramNameVerbose,
                      int min, int max, int defaultVal,
                      juce::String parameterLabel = juce::String(),
                      std::function<juce::String(int value, int maximumStringLength)> stringFromInt = nullptr,
                      std::function<int(const juce::String& text)> intFromString = nullptr)
            :   AudioParameterInt (paramNameVerbose, TRANS (paramNameVerbose), min, max, defaultVal,
                                   parameterLabel, stringFromInt, intFromString),
                Parameter (key, this,
                           AudioParameterInt::getNormalisableRange().convertTo0to1 (static_cast<float>(defaultVal)),
                           paramNameShort, paramNameVerbose),
                intToString (stringFromInt), stringToInt (intFromString)
        { }
        
        int getDefault() const { return juce::roundToInt (Parameter::getDenormalizedDefault()); }
        
        std::function< juce::String (int, int) > intToString;
        std::function< int (const juce::String&) > stringToInt;
        
        juce::ValueTree toValueTree() const override final
        {
            using namespace DefaultValueTreeIds;
            
            juce::ValueTree tree { Parameter_Int };
            
            tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
            tree.setProperty (ParameterValue, AudioParameterInt::get(), nullptr);
            tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
            tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);
            
            return tree;
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntParameter)
    };
    
    
//==============================================================================


    class BoolParameter    :        public juce::AudioParameterBool,
                                    public bav::Parameter
    {
        using AudioParameterBool = juce::AudioParameterBool;
        
    public:
        BoolParameter (int key,
                       juce::String paramNameShort, juce::String paramNameVerbose,
                       bool defaultVal,
                       juce::String parameterLabel = juce::String(),
                       std::function<juce::String(bool value, int maximumStringLength)> stringFromBool = nullptr,
                       std::function<bool(const juce::String& text)> boolFromString = nullptr)
             :  AudioParameterBool (paramNameVerbose, TRANS (paramNameVerbose), defaultVal, parameterLabel,
                                    stringFromBool, boolFromString),
                Parameter (key, this,
                           AudioParameterBool::getNormalisableRange().convertTo0to1 (static_cast<float>(defaultVal)),
                           paramNameShort, paramNameVerbose),
                boolToString (stringFromBool), stringToBool (boolFromString)
        { }
        
        bool getDefault() const { return Parameter::getNormalizedDefault() >= 0.5f; }
        
        std::function< juce::String (bool, int) > boolToString;
        std::function< bool (const juce::String& text) > stringToBool;
        
        juce::ValueTree toValueTree() const override final
        {
            using namespace DefaultValueTreeIds;
            
            juce::ValueTree tree { Parameter_Bool };
            
            tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
            tree.setProperty (ParameterValue, AudioParameterBool::get(), nullptr);
            tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
            tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);
            
            return tree;
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolParameter)
    };


//==============================================================================


    struct MeterParameter :     public FloatParameter
    {
        MeterParameter (int key,
                        juce::String paramNameShort, juce::String paramNameVerbose,
                        juce::NormalisableRange<float> nRange, float defaultVal, juce::String parameterLabel = juce::String(),
                        juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter,
                        std::function<juce::String(float value, int maximumStringLength)> stringFromValue = nullptr,
                        std::function<float(const juce::String& text)> valueFromString = nullptr)
        
        :   FloatParameter (key, paramNameShort, paramNameVerbose, nRange, defaultVal,
                            parameterLabel, parameterCategory, stringFromValue, valueFromString)
        { }
        
        juce::ValueTree toValueTree() const override final
        {
            juce::ValueTree tree { DefaultValueTreeIds::Parameter_Meter };
            
            tree.copyPropertiesFrom (FloatParameter::toValueTree(), nullptr);
            
            return tree;
        }
        
        bool isAutomatable() const override final { return false; }
    };


//==============================================================================


    struct GainMeterParameter : public MeterParameter
    {
        GainMeterParameter (int key,
                            juce::String paramNameShort, juce::String paramNameVerbose,
                            juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter)
        
        :   MeterParameter (key, paramNameShort, paramNameVerbose,
                            juce::NormalisableRange<float>(-60.0f, 0.0f, 0.01f),
                            -60.0f, TRANS ("dB"), parameterCategory,
                            ParameterValueConversionLambdas::gain_stringFromFloat,
                            ParameterValueConversionLambdas::gain_floatFromString)
        { }
    };

    
}  // namespace bav


