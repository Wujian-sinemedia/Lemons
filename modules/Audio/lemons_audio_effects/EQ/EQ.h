
#pragma once

namespace lemons::dsp::FX
{
/**
    An equalizer effect class.
    This class holds a collection of Filter effect objects and processes them in serial.
    @see Filter
 */
template < typename SampleType >
class EQ : public AudioEffect< SampleType >
{
public:
    using Band = FilterEngine< SampleType >;

    /** Creates a default EQ object. It will initially have no bands. */
    EQ() = default;

    /** Processes the EQ.
        Internally, this calls process() on each of the EQ's filters in serial.
     */
    void process (AudioBuffer< SampleType >& audio) final;

    /** Prepares the EQ. */
    void prepare (double samplerate, int blocksize) final;

    /** Adds a new band to the EQ.
        The EQ object will take ownership of the band passed to this function.
     */
    void addBand (Band* newFilter);

    /** Creates and adds a new band to the EQ.
        The arguments are all passed to the constructor of FX::Filter.
     */
    template < typename... Args >
    void addBand (Args&&... args)
    {
        addBand (new Band (std::forward< Args > (args)...));
    }

    /** Returns the total number of bands this EQ owns. */
    int getNumBands() const;

    /** Returns a band at a certain index in the EQ's internal array of bands.
        This is the least preferrable way of retrieving bands; I would recommend either getBandOfType() or getBandAtFrequency().
        The returned pointer may be null if no such index exists in the EQ's array of bands.
     */
    Filter< SampleType >* getBand (int index);

    /** Returns the first band found of the given type, if any.
        The returned pointer may be null if no bands have the requested type.
     */
    Filter< SampleType >* getBandOfType (FilterType type);

    /** Returns the first band found with the given frequency, if any.
        The returned pointer may be null if no bands have the requested frequency.
     */
    Filter< SampleType >* getBandAtFrequency (float freq);


private:
    void serialize (TreeReflector& ref) final;

    juce::OwnedArray< Band > bands;

    double lastSamplerate {44100.};
    int    lastBlocksize {512};
};

}  // namespace lemons::dsp::FX
