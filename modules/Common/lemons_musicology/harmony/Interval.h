#pragma once

namespace lemons
{

class Interval final
{
public:
    
    /*
     create from
     - num semitones
     - quality and interval
     */
    
    enum class Quality
    {
        Major,
        Minor,
        Perfect,
        Augmented,
        Diminished
    };
    
    [[nodiscard]] Quality getQuality() const;
    
    [[nodiscard]] int getNumSemitones() const { }
    
    // get string description
    
private:
    Quality quality;
};

}
