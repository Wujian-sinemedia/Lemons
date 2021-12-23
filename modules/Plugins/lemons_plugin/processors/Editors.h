/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

namespace lemons::plugin
{

/** Base class for plugin editors' GUI content components.
 */
class PluginGUI : public juce::Component
{
public:
    explicit PluginGUI (PluginState& s)
    : state(s)
    { }
    
protected:
    PluginState& state;
};


/** Base class for a plugin editor that simply holds a specified somponent.
    @tparam ContentComponentType The type of your plugin's main component. This must inherit from PluginGUI.
 */
template <typename ContentComponentType, LEMONS_MUST_INHERIT_FROM (ContentComponentType, PluginGUI)>
class PluginEditor final : public juce::AudioProcessorEditor
{
public:
	/** Creates a plugin editor. */
	explicit PluginEditor (ProcessorBase& p)
	    : AudioProcessorEditor (p), content(p.getState())
	{
        const auto& initialSize = p.getState().editorSize;
        
		jassert (initialSize.isValid());

		setResizable (true, true);

		const auto width  = initialSize.getWidth();
		const auto height = initialSize.getHeight();

		auto* c = getConstrainer();

		c->setMinimumSize (width / 2, height / 2);
		c->setMaximumSize (width * 2, height * 2);
		c->setFixedAspectRatio (initialSize.getAspectRatio());

		addAndMakeVisible (content);
        
        setSize (width, height);
	}

private:
	void resized() final
	{
		content.setBounds (getLocalBounds());
	}

	void paint (juce::Graphics& g) final
	{
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	}

	ContentComponentType content;

	juce::TooltipWindow tooltipWindow { this, 700 };
};

}  // namespace lemons::dsp
