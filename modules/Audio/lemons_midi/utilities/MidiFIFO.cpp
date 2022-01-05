/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


namespace lemons::midi
{

MidiFIFO::MidiFIFO (int maxNumMessages) { setSize (maxNumMessages); }

void MidiFIFO::setSize (int maxNumMessages)
{
	jassert (maxNumMessages > 0);

	clear();

	const auto messages = static_cast<size_t> (maxNumMessages);
	base.ensureSize (messages);
	copying.ensureSize (messages);
}

void MidiFIFO::clear()
{
	base.clear();
	copying.clear();
	numSamplesStored = 0;
}

int MidiFIFO::numStoredEvents() const noexcept { return base.getNumEvents(); }

int MidiFIFO::numStoredSamples() const noexcept { return numSamplesStored; }

void MidiFIFO::pushEvents (const MidiBuffer& source, int numSamples)
{
	base.addEvents (source, 0, numSamples, numSamplesStored);
	numSamplesStored += numSamples;
}

void MidiFIFO::popEvents (MidiBuffer& output, int numSamples)
{
	output.clear();
	output.addEvents (base, 0, numSamples, 0);

	// Move all the remaining events forward by the number of samples removed
	copying.clear();
	copying.addEvents (base, numSamples, numSamplesStored, -numSamples);

	base.swapWith (copying);
	numSamplesStored = std::max (0, numSamplesStored - numSamples);
}

}  // namespace lemons::midi

