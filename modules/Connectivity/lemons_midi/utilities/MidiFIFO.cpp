
namespace lemons::midi
{

MidiFIFO::MidiFIFO (int maxNumMessages) { setSize (maxNumMessages); }

void MidiFIFO::setSize (int maxNumMessages)
{
    const auto messages = static_cast<size_t> (maxNumMessages);
    base.ensureSize (messages);
    copying.ensureSize (messages);
}


void MidiFIFO::clear()
{
    base.clear();
    copying.clear();
    numStoredSamples = 0;
}


int MidiFIFO::numStoredEvents() const { return base.getNumEvents(); }


void MidiFIFO::pushEvents (const MidiBuffer& source, int numSamples)
{
    base.addEvents (source, 0, numSamples, numStoredSamples);
    numStoredSamples += numSamples;
}


void MidiFIFO::popEvents (MidiBuffer& output, int numSamples)
{
    output.clear();
    output.addEvents (base, 0, numSamples, 0);
    
    // Move all the remaining events forward by the number of samples removed
    copying.clear();
    copying.addEvents (base, numSamples, numStoredSamples, -numSamples);
    
    base.swapWith (copying);
    numStoredSamples = std::max (0, numStoredSamples - numSamples);
}

}
