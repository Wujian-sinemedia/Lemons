
#include <lemons_mobile_utils/lemons_mobile_utils.h>

namespace lemons::plugin
{
static inline bool isValidSize (const juce::Point< int >& size)
{
    return size.x > 0 && size.y > 0;
}

EditorBase::EditorBase (ProcessorBase& pbToUse, juce::Point< int > initialSize)
    : AudioProcessorEditor (pbToUse), pb (pbToUse)
{
    setResizable (true, true);

    if (const auto size = pb.getSavedEditorSize(); isValidSize (size))
    {
        initializeSize (size.x, size.y);
    }
    else
    {
        jassert (isValidSize (initialSize));
        initializeSize (initialSize.x, initialSize.y);
    }

    AutoLock::setEnabled (false);
}

void EditorBase::initializeSize (int width, int height)
{
    auto* c = getConstrainer();

    c->setMinimumSize (width / 2, height / 2);
    c->setMaximumSize (width * 2, height * 2);
    c->setFixedAspectRatio (static_cast< float > (width) / static_cast< float > (height));

    setSize (width, height);
}

void EditorBase::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void EditorBase::resized()
{
    pb.saveEditorSize (getWidth(), getHeight());
    resizeTriggered();
}

}  // namespace lemons::plugin
