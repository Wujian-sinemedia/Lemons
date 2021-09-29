namespace lemons::dsp::osc
{
template < typename SampleType >
ChoosableOscillator< SampleType >::ChoosableOscillator()
{
    setFrequency (440.f);
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::setOscType (OscType newType)
{
    type = newType;
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::process (AudioBuffer< SampleType >& output)
{
    output.clear();

    switch (type)
    {
        case (SineOsc) :
        {
            sine.process (output, false);
            saw.process (output, true);
            square.process (output, true);
            triangle.process (output, true);
            superSaw.process (output, true);
            return;
        }
        case (SawOsc) :
        {
            sine.process (output, true);
            saw.process (output, false);
            square.process (output, true);
            triangle.process (output, true);
            superSaw.process (output, true);
            return;
        }
        case (SquareOsc) :
        {
            sine.process (output, true);
            saw.process (output, true);
            square.process (output, false);
            triangle.process (output, true);
            superSaw.process (output, true);
            return;
        }
        case (TriangleOsc) :
        {
            sine.process (output, true);
            saw.process (output, true);
            square.process (output, true);
            triangle.process (output, false);
            superSaw.process (output, true);
            return;
        }
        case (SuperSawOsc) :
        {
            sine.process (output, true);
            saw.process (output, true);
            square.process (output, true);
            triangle.process (output, true);
            superSaw.process (output, false);
            return;
        }
    }
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::prepare (int blocksize, double samplerate)
{
    sine.prepare (samplerate, blocksize);
    saw.prepare (samplerate, blocksize);
    square.prepare (samplerate, blocksize);
    triangle.prepare (samplerate, blocksize);
    superSaw.prepare (samplerate, blocksize);

    prepared (blocksize);
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::setFrequency (float freqHz)
{
    sine.setFrequency (freqHz);
    saw.setFrequency (freqHz);
    square.setFrequency (freqHz);
    triangle.setFrequency (freqHz);
    superSaw.setFrequency (freqHz);

    freq = freqHz;
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::setDetuneAmount (int pitchSpreadCents)
{
    superSaw->setDetuneAmount (pitchSpreadCents);
}

template < typename SampleType >
void ChoosableOscillator< SampleType >::serialize (TreeReflector& ref)
{
    ref.add ("OscillatorType", type);

    const String freqProp   = "Frequency";
    const String detuneProp = "DetuneAmount";

    auto& tree = ref.getRawDataTree();

    if (ref.isSaving())
    {
        tree.setProperty (freqProp, freq, nullptr);
        tree.setProperty (detuneProp, superSaw->getPitchSpreadCents(), nullptr);
    }
    else
    {
        if (tree.hasProperty (freqProp))
            setFrequency (tree.getProperty (freqProp));

        if (tree.hasProperty (detuneProp))
            setDetuneAmount (tree.getProperty (detuneProp));
    }
}

template class ChoosableOscillator< float >;
template class ChoosableOscillator< double >;

}  // namespace bav::dsp::osc
