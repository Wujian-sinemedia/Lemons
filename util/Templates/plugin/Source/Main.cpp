#include <new_dsp/new_dsp.h>
#include <new_gui/new_gui.h>


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    using namespace New;

    return new plugin::ProcessorWithEditor< Processor, GUI > ({1060, 640});
}
