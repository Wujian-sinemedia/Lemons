/*
    bv_FFT: This is a wrapper class around several platform-specific FFT implementations that can be used in a generalized way.
    This is packaged as a separate JUCE module from bv_SharedCode, but has a dependancy on bv_SharedCode.
*/


/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_FFT
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_FFT
 description:        Ben Vining's wrapper class for FFT's
 dependencies:       bv_SharedCode
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

#include "bv_SharedCode/bv_SharedCode.h"


namespace bav::dsp
{
    

template typename<SampleType>
class FFT
{
public:
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
};
    

/// explicit instantiations...
template class FFT<float>;
template class FFT<double>;


}  // namespace
