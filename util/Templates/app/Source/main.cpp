#include <new_app_gui/new_app_gui.h>
#include <lemons_app_utils/lemons_app_utils.h>

struct NewApp : lemons::GuiApp< New::GUI >
{
    NewApp()
    : GuiApp< New::GUI > ("NewApp", "0.0.1", {1150, 700})
    {
    }
};

START_JUCE_APPLICATION (NewApp)
