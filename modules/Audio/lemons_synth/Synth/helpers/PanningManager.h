#pragma once

namespace lemons::dsp::synth
{

template <typename T>
using Array = juce::Array<T>;

template <typename SampleType>
class PanningManager
{
public:

	explicit PanningManager (SynthBase<SampleType>& b)
		: synth (b)
	{
	}

	void updateStereoWidth (int newWidth);

	void setLowestNote (int newLowestNote);
	int	 getLowestNote() const { return lowestPannedNote; }

private:

	friend class SynthBase<SampleType>;

	void prepare (int numVoices, bool clearArrays = true);
	void reset();

	int	 getNextPanVal();
	void panValTurnedOff (int panVal);

	void updatePanValueLookupTables (int newWidth);
	void mapArrayIndexes();
	int	 getClosestNewPanValFromOld (int oldPan);
	int	 findClosestValueInNewArray (int targetValue, Array<int>& newArray);

	SynthBase<SampleType>& synth;

	int stereoWidth { 100 };
	int lowestPannedNote { 0 };

	Array<int> arrayIndexesMapped;
	Array<int> possiblePanVals, panValsInAssigningOrder, unsentPanVals;
	Array<int> newPanVals, newUnsentVals;
	Array<int> distances;
};

}  // namespace lemons::dsp::synth
