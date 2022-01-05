#pragma once

namespace lemons::dsp::osc
{

template<typename SampleType, template<typename T> class OscType,
         LEMONS_MUST_INHERIT_FROM(OscType<SampleType>, Oscillator<SampleType>)>
class Engine : public dsp::StreamEngine<SampleType, OscType>
{
public:
    
    explicit Engine() = default;
    
    /** Updates the frequency of the owned oscillator object. */
    void setFrequency (float freqHz)
    {
        jassert (freqHz > 0.f);
        
        frequency = freqHz;
        
        const auto sr   = static_cast<SampleType> (this->getSamplerate());
        const auto freq = static_cast<SampleType> (frequency);
        
        for (auto* osc : this->streams)
            osc->setFrequency (freq, sr);
    }
    
    /** Returns the oscillator's current frequency. */
    [[nodiscard]] float getFrequency() const noexcept { return frequency; }
    
private:
    void prepared (int, double samplerate, int numChannels) final
    {
        this->streams.resize (numChannels);
        
        for (auto* osc : this->streams)
        {
            osc->resetPhase();
            osc->setFrequency (frequency, static_cast<SampleType> (samplerate));
        }
    }
    
    void onReset() final
    {
        for (auto* osc : this->streams)
            osc->resetPhase();
    }
    
    void released() final
    {
        for (auto* osc : this->streams)
            osc->resetPhase();
    }
    
    float frequency { 440.f };
};

}
