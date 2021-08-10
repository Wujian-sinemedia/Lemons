
namespace bav::dsp
{
void LFO::prepared (int blocksize)
{
    storage.setSize (1, blocksize, true, true, true);
}

void LFO::process (int numSamples)
{
    AudioBuffer< float > alias {storage.getArrayOfWritePointers(), 1, numSamples};
    osc::ChoosableOscillator< float >::process (alias);
}

}  // namespace bav::dsp
