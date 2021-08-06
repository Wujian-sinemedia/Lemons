
namespace bav::plugin
{
StateToggleButtons::StateToggleButtons (StateToggler& togglerToUse)
    : toggler (togglerToUse)
{
}

void StateToggleButtons::A_ButtonPressed()
{
    toggler.saveLastSelectedState();
    toggler.loadStateA();
}

void StateToggleButtons::B_ButtonPressed()
{
    toggler.saveLastSelectedState();
    toggler.loadStateB();
}

void StateToggleButtons::C_ButtonPressed()
{
    toggler.saveLastSelectedState();
    toggler.loadStateC();
}

}  // namespace bav::plugin
