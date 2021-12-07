#pragma once


namespace lemons::music
{

class KeySignature final
{
public:
    
    // operator ==, !=
    
    [[nodiscard]] constexpr bool isFlatKey() const { return false; }
    
    [[nodiscard]] constexpr bool isSharpKey() const { return true; }
    
    [[nodiscard]] bool containsPitchClass (int pitchClass) const
    {
        return false;
    }
    
    // get num sharps / flats
    
    // function to return a key sig object representing another key with a specified relationship -- the dominant, etc...
    
    // get major / minor root note
    // get pitch class for scale degree
    
private:
    
};

}
