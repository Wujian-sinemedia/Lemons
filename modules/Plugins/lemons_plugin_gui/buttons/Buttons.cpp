
namespace lemons::gui::components
{

ToggleButton::ToggleButton (plugin::BoolParameter& paramToUse, std::function<void (bool)> cb)
    : ToggleTextButton (paramToUse.getName(50),
                        paramToUse.get(),
                        [&] (bool state)
                        { clicked_callback (state); })
    , param (paramToUse)
    , callback (std::move (cb))
{
}

void ToggleButton::clicked_callback (bool state)
{
	param.set (state);
	callback (state);
}

}  // namespace lemons::gui
