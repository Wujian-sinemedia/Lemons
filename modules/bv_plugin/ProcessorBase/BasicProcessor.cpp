
namespace bav::dsp
{
BasicProcessorBase::BasicProcessorBase()
    : AudioProcessor (createBusProperties())
{
}

BasicProcessorBase::~BasicProcessorBase() { }

/*=========================================================================================*/

void BasicProcessorBase::prepareToPlay (double, int) { }
void BasicProcessorBase::releaseResources() { }

double BasicProcessorBase::getTailLengthSeconds() const { return 0.0; }

int          BasicProcessorBase::getNumPrograms() { return 1; }
int          BasicProcessorBase::getCurrentProgram() { return 0; }
void         BasicProcessorBase::setCurrentProgram (int) { }
const String BasicProcessorBase::getProgramName (int) { return {}; }
void         BasicProcessorBase::changeProgramName (int, const String&) { }

bool BasicProcessorBase::acceptsMidi() const { return true; }
bool BasicProcessorBase::producesMidi() const { return true; }
bool BasicProcessorBase::supportsMPE() const { return false; }
bool BasicProcessorBase::isMidiEffect() const { return false; }

const String BasicProcessorBase::getName() const { return "ProcessorBase"; }

bool BasicProcessorBase::hasEditor() const { return false; }

juce::AudioProcessorEditor* BasicProcessorBase::createEditor() { return nullptr; }

bool BasicProcessorBase::isBusesLayoutSupported (const BusesLayout& layout) const
{
    using Set = juce::AudioChannelSet;

    auto isValid = [] (const Set& set)
    { return set != Set::disabled(); };

    return isValid (layout.getMainInputChannelSet()) && isValid (layout.getMainOutputChannelSet());
}

juce::AudioProcessor::BusesProperties BasicProcessorBase::createBusProperties() const
{
    const auto stereo = juce::AudioChannelSet::stereo();

    return BusesProperties().withInput (TRANS ("Input"), stereo, true).withOutput (TRANS ("Output"), stereo, true);
}

void BasicProcessorBase::saveEditorSize (int width, int height)
{
    savedEditorSize.x = width;
    savedEditorSize.y = height;
}

juce::Point< int > BasicProcessorBase::getSavedEditorSize() const
{
    return savedEditorSize;
}

void BasicProcessorBase::getSavedEditorSize (int& width, int& height) const
{
    width  = savedEditorSize.x;
    height = savedEditorSize.y;
}

void BasicProcessorBase::repaintEditor()
{
    if (auto* editor = getActiveEditor())
        editor->repaint();
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

}  // namespace bav::dsp
