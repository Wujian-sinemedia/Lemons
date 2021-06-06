
namespace bav::dsp
{
ProcessorBase::ProcessorBase()
    : AudioProcessor (createBusProperties())
{
}

ProcessorBase::~ProcessorBase() { }

/*=========================================================================================*/

void ProcessorBase::prepareToPlay (double, int) { }
void ProcessorBase::releaseResources() { }

void ProcessorBase::processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    if (auto* b = getMainBypass())
        if (! b->get())
            b->set (true);
    
    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    if (auto* b = getMainBypass())
        if (! b->get())
            b->set (true);
    
    processBlockInternal (audio, midi);
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
    return getMainBypass();
}

double ProcessorBase::getTailLengthSeconds() const { return 0.0; }

void ProcessorBase::getStateInformation (juce::MemoryBlock&) { }
void ProcessorBase::setStateInformation (const void*, int) { }

int                ProcessorBase::getNumPrograms() { return 1; }
int                ProcessorBase::getCurrentProgram() { return 0; }
void               ProcessorBase::setCurrentProgram (int) { }
const String ProcessorBase::getProgramName (int) { return {}; }
void               ProcessorBase::changeProgramName (int, const String&) { }

bool ProcessorBase::acceptsMidi() const { return true; }
bool ProcessorBase::producesMidi() const { return true; }
bool ProcessorBase::supportsMPE() const { return false; }
bool ProcessorBase::isMidiEffect() const { return false; }

const String ProcessorBase::getName() const { return "ProcessorBase"; }

bool ProcessorBase::hasEditor() const { return false; }

juce::AudioProcessorEditor* ProcessorBase::createEditor() { return nullptr; }

bool ProcessorBase::isBusesLayoutSupported (const BusesLayout& layout) const
{
    using Set = juce::AudioChannelSet;

    auto isValid = [] (const Set& set)
    { return set != Set::disabled(); };

    return isValid (layout.getMainInputChannelSet()) && isValid (layout.getMainOutputChannelSet());
}

juce::AudioProcessor::BusesProperties ProcessorBase::createBusProperties() const
{
    const auto stereo = juce::AudioChannelSet::stereo();

    return BusesProperties().withInput (TRANS ("Input"), stereo, true).withOutput (TRANS ("Output"), stereo, true);
}

void ProcessorBase::saveEditorSize (int width, int height)
{
    savedEditorSize.x = width;
    savedEditorSize.y = height;
}

juce::Point< int > ProcessorBase::getSavedEditorSize() const
{
    return savedEditorSize;
}

void ProcessorBase::getSavedEditorSize (int& width, int& height) const
{
    width  = savedEditorSize.x;
    height = savedEditorSize.y;
}

void ProcessorBase::repaintEditor()
{
    if (auto* editor = getActiveEditor())
        editor->repaint();
}

}  // namespace bav::dsp
