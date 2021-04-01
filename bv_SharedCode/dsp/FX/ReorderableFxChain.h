
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
        
        // called in the subclass to apply the effect to the input audio. Output should be rendered in place.
        virtual void process (AudioBuffer& audio) = 0;
        
        
        virtual void bypassedBlock (const int numSamples)
        {
            juce::ignoreUnused (numSamples);
        }
        
        
        virtual void prepare (double samplerate, int blocksize)
        {
            juce::ignoreUnused (samplerate, blocksize);
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
        
        // adds an effect to the chain. Returns the actual number in the chain that the effect was placed in.
        int addEffect (Effect* effect, const int numberInChain, bool addAsBypassed = false)
        {
            const auto newNumber = assignNewEffectNumber (numberInChain);
            
            jassert (newNumber >= 0);
            
            effect->effectNumber = newNumber;
            
            effect->isBypassed = addAsBypassed;
            
            effects.add (effect);
            
            jassert (getEffect(newNumber) != nullptr);
            
            if (lastSamplerate > 0.0 && lastBlocksize > 0)
                getEffect(newNumber)->prepare (lastSamplerate, lastBlocksize);
            
            return newNumber;
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
            
            const auto initNum = first->effectNumber;
            first->effectNumber = second->effectNumber;
            second->effectNumber = initNum;
            return true;
        }
        
        bool swapTwoEffects (const int firstNumInChain, const int secondNumInChain)
        {
            return swapTwoEffects (getEffect(firstNumInChain), getEffect(secondNumInChain));
        }
        
        
        bool setEffectBypass (Effect* effect, const bool shouldBeBypassed)
        {
            if (effect == nullptr)
                return false;
            
            effect->isBypassed = shouldBeBypassed;
            return true;
        }
        
        bool setEffectBypass (const int numberInChain, const bool shouldBeBypassed)
        {
            return setEffectBypass (getEffect (numberInChain), shouldBeBypassed);
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
        
        
        void prepare (double samplerate, int blocksize)
        {
            jassert (samplerate > 0.0 && blocksize > 0);
            
            lastSamplerate = samplerate;
            lastBlocksize = blocksize;
            
            for (auto* effect : effects)
                effect->prepare (samplerate, blocksize);
        }
        
        
        /*
            The top-level rendering callback that renders the entire effects chain in order.
            Audio will be output in place.
         */
        void process (AudioBuffer& audio)
        {
            jassert (! effects.isEmpty());
            jassert (lastSamplerate > 0.0 && lastBlocksize > 0);
            
            audio.clear();
            
            for (int i = 0; i < numActiveEffects(); ++i)
                if (auto* effect = getEffect(i))
                    if (! effect->isBypassed)
                        effect->process (audio);
            
            const auto numSamples = audio.getNumSamples();
            
            for (auto* effect : effects)
                if (effect->isBypassed)
                    effect->bypassedBlock (numSamples);
        }
        
        
        /*
            Call this function to render the entire chain in reverse order.
         */
        void processInReverseOrder (AudioBuffer& audio)
        {
            jassert (! effects.isEmpty());
            jassert (lastSamplerate > 0.0 && lastBlocksize > 0);
            
            audio.clear();
            
            for (int i = numActiveEffects() - 1; i >= 0; --i)
                if (auto* effect = getEffect(i))
                    if (! effect->isBypassed)
                        effect->process (audio);
            
            const auto numSamples = audio.getNumSamples();
            
            for (auto* effect : effects)
                if (effect->isBypassed)
                    effect->bypassedBlock (numSamples);
        }
        
        
        /*
            Call this function to inform the FX chain when a bypassed block is recieved.
         */
        void bypassedBlock (const int numSamples)
        {
            for (auto* effect : effects)
                effect->bypassedBlock (numSamples);
        }
        
        
    private:
        
       // if the current effect number is out of range or taken, attempts to return the closest availabe effect number to the passed number.
       inline int assignNewEffectNumber (int requestedNumber)
       {
           if (effects.isEmpty())
               return requestedNumber;
           
           if (requestedNumber < 0)
               requestedNumber = 0;
           
           if (isEffectNumberAvailable (requestedNumber))
               return requestedNumber;
           
           int maxNumber = -1;
           
           for (auto* effect : effects)
               if (effect->effectNumber > maxNumber)
                   maxNumber = effect->effectNumber;
           
           for (int num = 0, p = 1, m = -1;
                num <= maxNumber;
                ++num, ++p, --m)
           {
               const auto lower = requestedNumber + m;
               
               if (lower >= 0 && isEffectNumberAvailable (lower))
                   return lower;
               
               const auto higher = requestedNumber + p;
               
               if (higher >= maxNumber && isEffectNumberAvailable (higher))
                   return higher;
           }
           
           return maxNumber + 1;
       }
                                           
       // returns true if the effect number is not assigned to an effect currently in this chain.
       inline bool isEffectNumberAvailable (const int numberInChain)
       {
           for (auto* effect : effects)
               if (effect->effectNumber == numberInChain)
                   return false;
           
           return true;
       }
        
        /*
        */
        
        double lastSamplerate = 0.0;
        int lastBlocksize = 0;
        
        juce::OwnedArray< Effect > effects;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableFxChain)
    };
    
    /// explicit instantiations
    template class ReorderableFxChain<float>;
    template class ReorderableFxChain<double>;
    
}  // namespace

