
namespace lemons::dsp
{
BasicProcessor::BasicProcessor (juce::AudioProcessor::BusesProperties busesLayout)
    : AudioProcessor (busesLayout)
{
}

void BasicProcessor::prepareToPlay (double, int) { }
void BasicProcessor::releaseResources() { }

double BasicProcessor::getTailLengthSeconds() const { return 0.0; }

int          BasicProcessor::getNumPrograms() { return 1; }
int          BasicProcessor::getCurrentProgram() { return 0; }
void         BasicProcessor::setCurrentProgram (int) { }
const String BasicProcessor::getProgramName (int) { return TRANS ("Program"); }
void         BasicProcessor::changeProgramName (int, const String&) { }

bool BasicProcessor::acceptsMidi() const { return true; }
bool BasicProcessor::producesMidi() const { return false; }
bool BasicProcessor::supportsMPE() const { return false; }
bool BasicProcessor::isMidiEffect() const { return false; }

const String BasicProcessor::getName() const { return TRANS ("ProcessorBase"); }

bool BasicProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* BasicProcessor::createEditor() { return new juce::GenericAudioProcessorEditor (*this); }

static inline bool isChannelsetValid (const juce::AudioChannelSet& set)
{
	return set != juce::AudioChannelSet::disabled();
}

bool BasicProcessor::isBusesLayoutSupported (const BusesLayout& layout) const
{
	return isChannelsetValid (layout.getMainInputChannelSet())
	    && isChannelsetValid (layout.getMainOutputChannelSet());
}

void BasicProcessor::repaintEditor() const
{
	juce::MessageManager::callAsync ([editor = juce::Component::SafePointer<juce::AudioProcessorEditor> (getActiveEditor())]
	                                 {
                                        if (auto* e = editor.getComponent())
                                            e->repaint(); });
}

}  // namespace lemons::dsp
