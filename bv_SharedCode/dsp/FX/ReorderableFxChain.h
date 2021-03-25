
/*
 This file defines a basic framework for a reorderable audio effects chain.
 */


namespace bav::dsp
{
    
    /// forward declaration...
    template<typename SampleType>
    class ReorderableFxChain;
    
    
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
        
        
    protected:
        // audio must be rendered in place!!!
        virtual void process (AudioBuffer& audio)
        {
            juce::ignoreUnused (audio);
        }
        
        
        virtual void bypassedBlock (const int numSamples)
        {
            juce::ignoreUnused (numSamples);
        }
        
        
    private:
        friend class ReorderableFxChain<SampleType>;
        
        int effectNumber;
        
        bool isBypassed = false;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableEffect)
    };
    
    /// explicit instantiations
    template class ReorderableEffect<float>;
    template class ReorderableEffect<double>;
    
    
    
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
        
        
        void addEffect (Effect* effect, const int numberInChain, bool addAsBypassed = false)
        {
            // edge case : new num is same as an existing num
            // edge case : new num is larger than 1+ largest existing num in chain...
            
            effect->effectNumber = numberInChain;
            
            if (addAsBypassed)
                effect->isBypassed = true;
        }
        
        
        void removeEffect (Effect* effect) { effects.removeObject (effect, true); }
        
        void removeEffect (const int numberInChain) { removeEffect (getEffect (numberInChain)); }
        
        void removeAllEffects() { effects.clear(); }
        
        
        Effect* getEffect (const int numberInChain)
        {
            jassert (numberInChain >= 0);
            
            for (auto* effect : effects)
                if (effect->effectNumber == numberInChain)
                    return effect;
            
            return nullptr;
        }
        
        
        bool swapTwoEffects (Effect* first, Effect* second)
        {
            if (first == nullptr || second == nullptr)
                return false;
            
            const int initNum = first->effectNumber;
            first->effectNumber = second->effectNumber;
            second->effectNumber = initNum;
            return true;
        }
        
        bool swapTwoEffects (const int firstNumInChain, const int secondNumInChain)
        {
            return swapTwoEffects (getEffect(firstNumInChain), getEffect(secondNumInChain));
        }
        
        
        void setEffectBypass (Effect* effect, const bool shouldBeBypassed)
        {
            effect->isBypassed = shouldBeBypassed;
        }
        
        void setEffectBypass (const int numberInChain, const bool shouldBeBypassed)
        {
            setEffectBypass (getEffect (numberInChain), shouldBeBypassed);
        }
        
        
        int numActiveEffects() const noexcept
        {
            int numActive = 0;
            
            for (auto* effect : effects)
                if (! effect->isBypassed)
                    ++numActive;
            
            return numActive;
        }
        
        
        void bypassAll()
        {
            for (auto* effect : effects)
                effect->isBypassed = true;
        }
        
        void unBypassAll()
        {
            for (auto* effect : effects)
                effect->isBypassed = false;
        }
        
        
        /*
         The top-level rendering callback that renders the entire effects chain in order.
         Audio will be output in place.
         */
        void process (AudioBuffer& audio)
        {
            audio.clear();
            
            for (int i = 0; i < numActiveEffects(); ++i)
                if (auto* effect = getEffect(i))
                    if (! effect->isBypassed)
                        effect->process (audio);
        }
        
        
        /*
         call this function to render the entire chain in reverse order.
         */
        void processInReverseOrder (AudioBuffer& audio)
        {
            audio.clear();
            
            for (int i = numActiveEffects() - 1; i >= 0; --i)
                if (auto* effect = getEffect(i))
                    if (! effect->isBypassed)
                        effect->process (audio);
        }
        
        
        /*
         Call this function to inform the FX chain when a bypassed block is recieved.
         */
        void bypassedBlock (const int numSamples)
        {
            for (int i = 0; i < numActiveEffects(); ++i)
                if (auto* effect = getEffect(i))
                    effect->bypassedBlock (numSamples);
        }
        
        
    private:
        
        juce::OwnedArray< Effect > effects;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableFxChain)
    };
    
    /// explicit instantiations
    template class ReorderableFxChain<float>;
    template class ReorderableFxChain<double>;
    
}  // namespace

