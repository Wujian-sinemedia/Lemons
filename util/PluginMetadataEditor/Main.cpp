#include <metadata_editor_gui/metadata_editor_gui.h>
#include <lemons_gui/lemons_gui.h>


namespace lemons
{
using LemonsPluginMetadataEditor = GuiApp<PluginMetadataEditor::gui::DocumentComponent>;
}


START_JUCE_APPLICATION (lemons::LemonsPluginMetadataEditor)
