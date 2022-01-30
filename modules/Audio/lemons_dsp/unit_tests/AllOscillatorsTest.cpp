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
 * ======================================================================================
 */

namespace lemons::tests
{

template <typename SampleType>
void AllOscillatorsTest<SampleType>::runTest()
{
	beginTest (getName());

	{
		const auto subtest = beginSubtest ("Sine");
		runOscillatorTest (sine);
	}

	{
		const auto subtest = beginSubtest ("Saw");
		runOscillatorTest (saw);
	}

	{
		const auto subtest = beginSubtest ("Square");
		runOscillatorTest (square);
	}

	{
		const auto subtest = beginSubtest ("Triangle");
		runOscillatorTest (triangle);
	}

	runOtherTests();
}

template struct AllOscillatorsTest<float>;
template struct AllOscillatorsTest<double>;

}  // namespace lemons::tests
