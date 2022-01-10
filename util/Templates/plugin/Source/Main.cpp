#include <new_dsp/new_dsp.h>
#include <new_gui/new_gui.h>


namespace New
{
using Plugin = plugin::ProcessorWithEditor<Processor, GUI>;
}

LEMONS_DEFINE_PLUGIN_CREATOR (New::Plugin)
