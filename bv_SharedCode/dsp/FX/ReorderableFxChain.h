
/*
    This file defines a basic framework for a reorderable audio effects chain.
*/


namespace bav::dsp
{
    
    /*
        This base class defines the basic interface any processor must use to be a member of a reorderable FX chain.
        The FX chain owns and manages a collection of these objects, and will delete them when they are removed.
    */
    template<typename SampleType>
    class ReorderableEffect
    {
        using AudioBuffer = juce::AudioBuffer<SampleType>;
        
    public:
        ReorderableEffect() { }
        
        virtual ~ReorderableEffect() = default;
        
        
        int number() const noexcept { return effectNumber; }
        
        bool isBypassed() const noexcept { return effectNumber > -1; }
        
        virtual void process (const AudioBuffer& input, AudioBuffer& output)
        {
            juce::ignoreUnused (input, output);
        }
        
        
        virtual void bypassedBlock (const int numSamples)
        {
            juce::ignoreUnused (numSamples);
        }
        
        
    private:
        friend class ReorderableFxChain<SampleType>;
        
        int effectNumber;  // this is the effect's number in the chain. Set this to -1 if the effect is temporarily bypassed.
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableEffect)
    };
    
    
    
    /*
        Base class for an engine object that owns and manages a series of individual effects processors, and can reorder them dynamically.
    */
    template<typename SampleType>
    class ReorderableFxChain
    {
        using Effect = ReorderableEffect<SampleType>;
        
        using AudioBuffer = juce::AudioBuffer<SampleType>;
        
        
    public:
        ReorderableFxChain() { }
        
        virtual ~ReorderableFxChain() = default;
        
        
        void addEffect (Effect* effect, const int numberInChain)
        {
            // edge case : new num is same as an existing num
            // edge case : new num is larger than 1+ largest existing num in chain...
        }
        
        
        void removeEffect (Effect* effect) { effects.removeObject (effect, true); }
        
        void removeEffect (const int numberInChain) { removeEffect (getEffect (numberInChain)); }
        
        void removeAllEffects() { effects.clear(); }
        
        
        Effect* getEffect (const int numberInChain)
        {
            jassert (numberInChain > -1); // this can't reliably find a specific bypassed effect, since they ALL have number -1...
            
            for (auto* effect : effects)
                if (effect->effectNumber == numberInChain)
                    return effect;
            
            return nullptr;
        }
        
        
        void swapTwoEffects (Effect* first, Effect* second)
        {
            const int initNum = first->effectNumber;
            first->effectNumber = second->effectNumber;
            second->effectNumber = initNum;
        }
        
        void swapTwoEffects (const int firstNumInChain, const int secondNumInChain)
        {
            swapTwoEffects (getEffect(firstNumInChain), getEffect(secondNumInChain));
        }
        
        
        int numActiveEffects() const noexcept
        {
            int numActive = 0;
            
            for (auto* effect : effects)
                if (! effect->isBypassed())
                    ++numActive;
            
            return numActive;
        }
        
        
        /*
            The top-level rendering callback that renders the entire effects chain in order.
        */
        void process (const AudioBuffer& input, AudioBuffer& output)
        {
            for (int i = 0; i < numActiveEffects(); ++i)
            {
                getEffect(i)->process (input, output);
            }
        }
        
        
        void processBypassed (const int numSamples)
        {
            for (int i = 0; i < numActiveEffects(); ++i)
            {
                getEffect(i)->bypassedBlock (numSamples);
            }
        }
        
        
    private:
        
        juce::OwnedArray< Effect > effects;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableFxChain)
    };
    
}  // namespace
