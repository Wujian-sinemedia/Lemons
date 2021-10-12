
#include <lemons_mobile_utils/lemons_mobile_utils.h>

namespace lemons::plugin
{
[[nodiscard]] static inline bool isValidSize (const juce::Point< int >& size) noexcept
{
    return size.x > 0 && size.y > 0;
}

static inline void initializeSize (juce::AudioProcessorEditor& editor, const juce::Point< int >& size)
{
    const auto width  = size.x;
    const auto height = size.y;

    auto* c = editor.getConstrainer();

    c->setMinimumSize (width / 2, height / 2);
    c->setMaximumSize (width * 2, height * 2);
    c->setFixedAspectRatio (static_cast< float > (width) / static_cast< float > (height));

    editor.setSize (width, height);
}

EditorBase::EditorBase (ProcessorBase& pbToUse, const juce::Point< int >& initialSize)
    : AudioProcessorEditor (pbToUse), pb (pbToUse)
{
    setResizable (true, true);

    if (const auto size = pb.getSavedEditorSize(); isValidSize (size))
    {
        initializeSize (*this, size);
    }
    else
    {
        jassert (isValidSize (initialSize));
        initializeSize (*this, initialSize);
    }

    AutoLock::setEnabled (false);
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
