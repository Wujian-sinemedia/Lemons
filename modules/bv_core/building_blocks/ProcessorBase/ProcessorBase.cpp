
namespace bav::dsp
{

ProcessorBase::ProcessorBase()
: AudioProcessor (createBusProperties())
{ }

ProcessorBase::~ProcessorBase() { }

/*=========================================================================================*/

void ProcessorBase::prepareToPlay (double, int) { }
void ProcessorBase::releaseResources() { }

void ProcessorBase::processBlock (juce::AudioBuffer< float >&, juce::MidiBuffer&) { }

double ProcessorBase::getTailLengthSeconds() const { return 0.0; }

void ProcessorBase::getStateInformation (juce::MemoryBlock&) { }
void ProcessorBase::setStateInformation (const void*, int) { }

int                ProcessorBase::getNumPrograms() { return 1; }
int                ProcessorBase::getCurrentProgram() { return 0; }
void               ProcessorBase::setCurrentProgram (int) { }
const juce::String ProcessorBase::getProgramName (int) { return {}; }
void               ProcessorBase::changeProgramName (int, const juce::String&) { }

bool ProcessorBase::acceptsMidi() const { return true; }
bool ProcessorBase::producesMidi() const { return true; }
bool ProcessorBase::supportsMPE() const { return false; }
bool ProcessorBase::isMidiEffect() const { return false; }

const juce::String ProcessorBase::getName() const { return "ProcessorBase"; }

bool                        ProcessorBase::hasEditor() const { return false; }

juce::AudioProcessorEditor* ProcessorBase::createEditor() { return nullptr; }

bool ProcessorBase::isBusesLayoutSupported(const BusesLayout& layout) const
{
    using Set = juce::AudioChannelSet;
    
    auto isValid = [](const Set& set) { return set != Set::disabled(); };
    
    return isValid (layout.getMainInputChannelSet()) && isValid (layout.getMainOutputChannelSet());
}

void ProcessorBase::saveEditorSize (int width, int height)
{
    savedEditorSize.x = width;
    savedEditorSize.y = height;
}

juce::Point<int> ProcessorBase::getSavedEditorSize() const { return savedEditorSize; }

juce::AudioProcessor::BusesProperties ProcessorBase::createBusProperties() const
{
    const auto stereo = juce::AudioChannelSet::stereo();
    
    return BusesProperties().withInput (TRANS ("Input"), stereo, true)
    .withOutput (TRANS ("Output"), stereo, true);
}

}  //namespace
