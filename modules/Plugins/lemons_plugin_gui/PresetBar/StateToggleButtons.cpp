
namespace lemons::plugin
{
StateToggleButtons::StateToggleButtons (StateToggler& togglerToUse)
    : toggler (togglerToUse)
{
    gui::addAndMakeVisible (this, a_button, b_button, c_button);
}

void StateToggleButtons::resized()
{
    using namespace gui;

    constexpr auto oneThird  = 1.f / 3.f;
    constexpr auto twoThirds = 2.f / 3.f;

    if (vertical)
    {
        scale (a_button, {0.f, 0.f, 1.f, oneThird});
        scale (b_button, {0.f, oneThird, 1.f, oneThird});
        scale (c_button, {0.f, twoThirds, 1.f, oneThird});
    }
    else
    {
        scale (a_button, {0.f, 0.f, oneThird, 1.f});
        scale (b_button, {oneThird, 0.f, oneThird, 1.f});
        scale (c_button, {twoThirds, 0.f, oneThird, 1.f});
    }
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

bool StateToggleButtons::isVertical() const
{
    return vertical;
}

void StateToggleButtons::setVertical (bool shouldBeVertical)
{
    vertical = shouldBeVertical;
    resized();
    repaint();
}

}  // namespace bav::plugin
