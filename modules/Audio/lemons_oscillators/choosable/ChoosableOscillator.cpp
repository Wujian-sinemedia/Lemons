namespace lemons::dsp::osc
{

template <typename SampleType>
void ChoosableOscillator<SampleType>::setOscType (OscType newType)
{
	type = newType;
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::process (AudioBuffer<SampleType>& output)
{
	output.clear();

	switch (type)
	{
		case (OscType::Sine) :
		{
			sine.process (output, false);
			saw.process (output, true);
			square.process (output, true);
			triangle.process (output, true);
			superSaw.process (output, true);
			return;
		}
		case (OscType::Saw) :
		{
			sine.process (output, true);
			saw.process (output, false);
			square.process (output, true);
			triangle.process (output, true);
			superSaw.process (output, true);
			return;
		}
		case (OscType::Square) :
		{
			sine.process (output, true);
			saw.process (output, true);
			square.process (output, false);
			triangle.process (output, true);
			superSaw.process (output, true);
			return;
		}
		case (OscType::Triangle) :
		{
			sine.process (output, true);
			saw.process (output, true);
			square.process (output, true);
			triangle.process (output, false);
			superSaw.process (output, true);
			return;
		}
		case (OscType::SuperSaw) :
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

template <typename SampleType>
void ChoosableOscillator<SampleType>::prepare (int blocksize, double samplerate)
{
	sine.prepare (samplerate, blocksize);
	saw.prepare (samplerate, blocksize);
	square.prepare (samplerate, blocksize);
	triangle.prepare (samplerate, blocksize);
	superSaw.prepare (samplerate, blocksize);

	prepared (blocksize);
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::setFrequency (float freqHz)
{
	sine.setFrequency (freqHz);
	saw.setFrequency (freqHz);
	square.setFrequency (freqHz);
	triangle.setFrequency (freqHz);
	superSaw.setFrequency (freqHz);

	freq = freqHz;
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::setDetuneAmount (int pitchSpreadCents)
{
	superSaw->setDetuneAmount (pitchSpreadCents);
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::serialize (TreeReflector& ref)
{
	ref.add ("OscillatorType", type);

	ref.addLambdaSet<float> (
	    "Frequency",
	    [&]
	    { return freq; },
	    [&] (float& f)
	    { setFrequency (f); });

	ref.addLambdaSet<int> (
	    "DetuneAmount",
	    [&]
	    { return superSaw->getPitchSpreadCents(); },
	    [&] (int& c)
	    { setDetuneAmount (c); });
}

template class ChoosableOscillator<float>;
template class ChoosableOscillator<double>;

}  // namespace lemons::dsp::osc
