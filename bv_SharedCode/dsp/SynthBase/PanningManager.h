/*
    Part of module: bv_Harmonizer
    Direct parent file: bv_Harmonizer.h
    Classes: PanningManager
 */

#pragma once


namespace bav
{
    
/* PanningManager: helper class to keep track of possible midi panning values within a given stero width, evenly spaced for the current number of harmony voices.
*/
class PanningManager
{
    
    using Array = juce::Array<int>;
    
    
public:
    
    PanningManager(): lastRecievedStereoWidth(64), currentNumVoices(0) { }
    ~PanningManager() { }
    
    void releaseResources()
    {
        panValsInAssigningOrder.clear();
        arrayIndexesMapped.clear();
        unsentPanVals.clear();
        possiblePanVals.clear();
        newPanVals.clear();
        newUnsentVals.clear();
        distances.clear();
    }
    
    void prepare (const int numVoices, bool clearArrays = true)
    {
        jassert (numVoices > 0);
        
        panValsInAssigningOrder.ensureStorageAllocated(numVoices);
        arrayIndexesMapped.ensureStorageAllocated(numVoices);
        unsentPanVals.ensureStorageAllocated(numVoices);
        possiblePanVals.ensureStorageAllocated(numVoices);
        newPanVals.ensureStorageAllocated(numVoices);
        newUnsentVals.ensureStorageAllocated(numVoices);
        distances.ensureStorageAllocated(numVoices);
        
        if (clearArrays)
        {
            panValsInAssigningOrder.clearQuick();
            arrayIndexesMapped.clearQuick();
            unsentPanVals.clearQuick();
            possiblePanVals.clearQuick();
            newPanVals.clearQuick();
            newUnsentVals.clearQuick();
            distances.clearQuick();
        }
        
        currentNumVoices = numVoices;
        
        mapArrayIndexes();
        updateStereoWidth(lastRecievedStereoWidth);
    }
    
    // used to update the width of the stereo field of currently available / valid pan values
    void updateStereoWidth(const int newWidth)
    {
        jassert (currentNumVoices > 0);
        
        lastRecievedStereoWidth = newWidth;
        
        const auto rangeMultiplier = newWidth * 0.01f;
        const auto range_extent = 63.5f * rangeMultiplier;
        const auto maxPan = 63.5f + range_extent;
        const auto minPan = 63.5f - range_extent;
        jassert (maxPan <= 127.0f && minPan >= 0.0f);
        const auto increment = (maxPan - minPan) / currentNumVoices;
        const auto halfIncrement = increment * 0.5f;
        
        possiblePanVals.clearQuick();
        
        for (int i = 0; i < currentNumVoices; ++i)
            possiblePanVals.add (juce::roundToInt (minPan + (i * increment) + halfIncrement));
        
        // reorder pan values into "assigning order" -- center out, by writing from the possiblePanVals array to the panValsInAssigningOrder array in the array index order held in arrayIndexesMapped
        panValsInAssigningOrder.clearQuick();
        
        mapArrayIndexes();
        
        for (int index : arrayIndexesMapped)
            panValsInAssigningOrder.add (possiblePanVals.getUnchecked(index));
        
        jassert (! panValsInAssigningOrder.isEmpty());
        
        if (unsentPanVals.isEmpty())
        {
            reset();
            return;
        }
        
        // the # of values we actually transfer to the I/O array being read from should correspond to the number of unsent pan vals left now -- ie, if some voices are already on, etc. And the values put in the I/O array should also be the values out of the panValsInAssigningOrder array that are closest to the old values from unsentPanVals...
        
        // make copy of panValsInAssigningOrder bc items will be removed during the searching process below
        
        newPanVals.clearQuick();
        
        for (int newPan : panValsInAssigningOrder)
            newPanVals.add (newPan);
        
        newUnsentVals.clearQuick();
        
        for (int oldPan : unsentPanVals)
            newUnsentVals.add (findClosestValueInNewArray (oldPan, newPanVals));
        
        newUnsentVals.removeAllInstancesOf (-1);
        
        if (newUnsentVals.isEmpty())
        {
            reset();
            return;
        }
        
        // transfer to I/O array we will be actually reading from
        unsentPanVals.clearQuick();
        
        for (int newPan : newUnsentVals)
            unsentPanVals.add (newPan);
        
        jassert (! unsentPanVals.isEmpty());
    }
    
    
    // returns next available panning value. Panning values are assigned from the "middle out" - the first pan values sent will be toward the center of the stereo field, working outwards to either side as more pan values are requested
    int getNextPanVal()
    {
        if (unsentPanVals.isEmpty())
            reset();
        
        jassert (! unsentPanVals.isEmpty());
        
        return unsentPanVals.removeAndReturn(0);
    }
    
    
    // used to tell the PanningManager when a previously assigned panningvalue is turned off - ie, is now available again for another voice 
    void panValTurnedOff (int panVal)
    {
        // this function is called when a pan value is turned off and is available again for assigning. This function attempts to reinsert the pan value into unsentPanVals with respect to the order the values are in in panValsInAssigningOrder
        
        const int targetindex = panValsInAssigningOrder.indexOf (panVal);
        
        if (targetindex == -1) // targetindex will be -1 if the turned off pan val is not in panValsInAssigningOrder. in this case, do nothing.
            return;
        
        if (unsentPanVals.isEmpty())
        {
            unsentPanVals.add (panVal);
            return;
        }
        
        int i = 0;
        bool addedIt = false;
        
        do {
            if (i >= unsentPanVals.size())
                break;
            
            if (panValsInAssigningOrder.indexOf (unsentPanVals.getUnchecked(i)) > targetindex)
            {
                unsentPanVals.insert (i, panVal);
                addedIt = true;
                break;
            }
            else
            {
                ++i;
            }
        }
        while (i < currentNumVoices);
        
        if (! addedIt)
            unsentPanVals.add (panVal);
    }
    
    
    // used when updating stereo width -- voices should grab the new pan val that's closest to their old pan val
    int getClosestNewPanValFromOld (int oldPan)
    {
        // find & return the element in readingFrom array that is the closest to oldPan, then remove that val from unsentPanVals
        // this is normally used with the unsentPanVals array, but the same function can also be used in the updating of the stereo width, to identify which new pan values should be sent to the unsentPanVals array itself, based on which new pan values are closest to the ones that were already in unsentPanVals.
        
        if (unsentPanVals.size() < 2)
            return getNextPanVal();
        
        return findClosestValueInNewArray (oldPan, unsentPanVals);
    }
    
    
    // tells the PanningManager that all voices have been turned off -- ie, all the pan vals are available again
    void reset()
    {
        if (panValsInAssigningOrder.isEmpty())
            return;
        
        unsentPanVals.clearQuick();
        
        for (int pan : panValsInAssigningOrder)
            unsentPanVals.add (pan);
    }
    

    int getCurrentStereoWidth() const { return lastRecievedStereoWidth; }
    
    int getCurrentNumVoices()   const noexcept { return currentNumVoices; }
    
    
private:
    
    Array panValsInAssigningOrder; // this array stores the pan values in the order they will be sent out, ie "middle out". Index 0 contains 64, and the highest two indices will contain 0 and 127 [if the stereo width is 100]
    
    Array arrayIndexesMapped; // this array is used to facilitate the transfer of values from possiblePanVals to panValsInAssigningOrder
    
    Array unsentPanVals; // this is the array we will actually be reading pan vals from! the others are for sorting.
    
    
    int lastRecievedStereoWidth;
    int currentNumVoices;
    
    void mapArrayIndexes()
    {
        /* In my updateStereoWidth() function, possible panning values are written to the possiblePanVals array in order from least to greatest absolute value. Index 0 will contain the smallest midiPan value, and the highest index will contain the largest midiPan value.
         
         When a new midiPan value is requested with getNextPanVal(), I want to assign them from the "center out" -- so that the first voice that turns on will be the one in the center, then the sides get added as more voices turn on.
         
         So I need to transfer the values in possiblePanVals into another array - panValsInAssigningOrder - which will hold the panning values in order so that index 0 contains 64, index 1 contains 72, index 2 contains 52... index 10 contains 127 and index 11 contains 0. [for 12 voices]
         
         In order to transfer the panning values from array to array like this, I need to essentially have a list of array indices of possiblePanVals to read from, in order of the panValsInAssigningOrder indices I'll be writing to. IE, in this list, if I'm working with 12 voices, index 0 will contain 6, meaning that I want to write the value in index 6 of possiblePanVals to index 0 of panValsInAssigningOrder.
         
         I'm storing this list in another array called arrayIndexesMapped.
         */
        
        arrayIndexesMapped.clearQuick();
        
        const int middleIndex = currentNumVoices > 1 ? juce::roundToInt (floor (currentNumVoices * 0.5f)) : 0;
        
        arrayIndexesMapped.add (middleIndex);
        
        int i = 1;
        int p = 1;
        int m = -1;
        
        while (i < currentNumVoices)
        {
            if (i % 2 == 0) // i is even
            {
                const int newI = middleIndex + p;
                
                if (newI > currentNumVoices)
                    continue;
                
                arrayIndexesMapped.add(newI);
                ++p;
            }
            else // i is odd
            {
                const int newI = middleIndex + m;
                
                if (newI < 0)
                    continue;
                
                arrayIndexesMapped.add(newI);
                --m;
            }
            ++i;
        }
    }
    
    int findClosestValueInNewArray (int targetValue, Array& newArray)
    {
        if (newArray.isEmpty())
            return -1;
        
        distances.clearQuick();
        
        for (int newVal : newArray)
        {
            const int distance = abs (targetValue - newVal);
            distances.add (distance);
        }
        
        return newArray.removeAndReturn (static_cast<int> (std::distance (distances.begin(),
                                                                          std::min_element (distances.begin(), distances.end()))));
    }
    
    Array possiblePanVals;
    
    Array newPanVals;
    Array newUnsentVals;
    
    Array distances;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanningManager)
};


} // namespace
