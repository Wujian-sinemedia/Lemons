#pragma once

#include <lemons_plugin/lemons_plugin.h>


namespace lemons::plugin
{
class StateToggleButtons : public juce::Component
{
public:
	//StateToggleButtons (StateToggles& togglerToUse);

	bool isVertical() const;
	void setVertical (bool shouldBeVertical);

private:
	void resized() final;

	//StateToggles& toggler;

//	gui::TextButton a_button { "A", [this]
//		                       { toggler.loadState (0); } };
//	gui::TextButton b_button { "B", [this]
//		                       { toggler.loadState (1); } };
//	gui::TextButton c_button { "C", [this]
//		                       { toggler.loadState (2); } };

	bool vertical { false };
};

}  // namespace lemons::plugin
