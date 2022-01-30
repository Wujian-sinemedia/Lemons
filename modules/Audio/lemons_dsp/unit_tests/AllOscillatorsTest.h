/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#pragma once

namespace lemons::tests
{

template <typename SampleType>
struct AllOscillatorsTest : public DspTest
{
public:

	using DspTest::DspTest;

private:

	void runTest() final;

	virtual void runOscillatorTest (dsp::Oscillator<SampleType>& osc) = 0;

	virtual void runOtherTests() { }

	dsp::osc::Sine<SampleType>	   sine;
	dsp::osc::Saw<SampleType>	   saw;
	dsp::osc::Square<SampleType>   square;
	dsp::osc::Triangle<SampleType> triangle;
};

}  // namespace lemons::tests
