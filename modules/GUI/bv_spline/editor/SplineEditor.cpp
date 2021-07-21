
namespace bav::spline
{
Editor::Select::Select (Editor& e) : editor (e)
{
}

void Editor::Select::setX (float xP) noexcept
{
    const auto bounds = editor.getAdjustedBounds();
    x                 = (xP - bounds.getX()) / bounds.getWidth();
}

void Editor::Select::operator()() { editor.knots.select (getRange()); }

bool Editor::Select::move (const Point& drag) noexcept
{
    return editor.knots.drag (drag);
}

void Editor::Select::deselect() noexcept { editor.knots.deselect(); }

void Editor::Select::mouseExited()
{
    x = -1;
}

void Editor::Select::addWidth (float w) { width = juce::jlimit (editor.scrollSensitivity, 1.f, width + w); }

juce::Range< float > Editor::Select::getRange() const noexcept
{
    return {
        juce::jlimit (0.f, 1.f, x - width),
        juce::jlimit (0.f, 1.f, x + width)};
}

void Editor::Select::paint (juce::Graphics& g)
{
    if (x == -1.f) return;
    g.setColour (editor.attributes.selection.color);

    const auto bounds = editor.getAdjustedBounds();

    const auto w       = bounds.getWidth();
    const auto boundsX = bounds.getX();

    auto xN     = juce::jlimit (0.f, 1.f, x - width) * w + boundsX;
    auto rightN = juce::jlimit (0.f, 1.f, x + width) * w + boundsX;

    juce::Rectangle< float > area (xN,
                                   bounds.getY(),
                                   rightN - xN,
                                   bounds.getHeight());

    g.fillRect (area);
}

Editor::Attributes::Attributes()
{
    border.thickness    = 2.f;
    point.thickness     = 3.f;
    curve.thickness     = 1.5f;
    selection.thickness = 5.f;

    border.color     = juce::Colours::cadetblue;
    point.color      = juce::Colours::cadetblue;
    curve.color      = juce::Colours::yellowgreen;
    selection.color  = juce::Colours::yellowgreen.withAlpha (.2f);
    background.color = juce::Colours::cadetblue.darker (2);

    pntrs.push_back ({&border});
    pntrs.push_back ({&point});
    pntrs.push_back ({&curve});
    pntrs.push_back ({&selection});
    pntrs.push_back ({&background});
}

float Editor::Attributes::getThickestThickness() const
{
    auto max = border.thickness;

    for (auto& pntr : pntrs)
        if (pntr->thickness > max)
            max = pntr->thickness;

    return max;
}

Editor::Editor (Processor& splineProcessor)
    : spline (splineProcessor)
{
}

void Editor::setBorderColour (Color c)
{
    attributes.border.color = c;
    repaint();
}

void Editor::setPointColour (Color c)
{
    attributes.point.color = c;
    repaint();
}

void Editor::setCurveColour (Color c)
{
    attributes.curve.color = c;
    repaint();
}

void Editor::setSelectionColour (Color c)
{
    attributes.selection.color = c;
    repaint();
}

void Editor::setBackgroundColour (Color c)
{
    attributes.background.color = c;
    repaint();
}

void Editor::setBorderThickness (float pxl)
{
    attributes.border.thickness = pxl;
    resized();
}

void Editor::setPointThickness (float pxl)
{
    attributes.point.thickness = pxl;
    resized();
}

void Editor::setCurveThickness (float pxl)
{
    attributes.curve.thickness = pxl;
    resized();
}

void Editor::setSelectionThickness (float pxl)
{
    attributes.selection.thickness = pxl;
    resized();
}

inline auto to_size_f (int idx)
{
    return static_cast< std::vector< float >::size_type > (idx);
}

void Editor::paint (juce::Graphics& g)
{
    g.setImageResamplingQuality (juce::Graphics::lowResamplingQuality);

    const auto bounds = getAdjustedBounds();

    g.setColour (attributes.background.color);
    g.fillRect (bounds);

    g.setColour (attributes.border.color);
    g.drawRect (bounds, attributes.border.thickness);

    g.setColour (attributes.curve.color);

    auto yStart = points[0];

    for (auto i = 1; i < static_cast< decltype (i) > (points.size()); ++i)
    {
        const auto          xEnd   = i + bounds.getX();
        const auto          xStart = i + bounds.getX() - 1;
        const auto          yEnd   = points[to_size_f (i)];
        juce::Line< float > cLine (xStart, yStart, xEnd, yEnd);
        g.drawLine (cLine, attributes.curve.thickness);
        yStart = points[to_size_f (i)];
    }

    select.paint (g);

    g.setColour (attributes.point.color);

    for (auto& k : knots.knots)
    {
        const auto knot = denormalizeKnot (k, bounds);

        const auto& attribute = k.isSelected() ? attributes.selection : attributes.point;

        drawPoint (knot, attribute, g);
    }
}

Point Editor::denormalizeKnot (const Knot&                     knot,
                               const juce::Rectangle< float >& bounds) const noexcept
{
    return {
        bounds.getX() + bounds.getWidth() * knot.x,
        bounds.getY() + bounds.getHeight() * knot.y};
}

void Editor::drawPoint (const Point&     knot,
                        const Attribute& attribute,
                        juce::Graphics&  g)
{
    const auto width = attribute.thickness * 2.f + 1.f;

    g.fillEllipse (knot.x - attribute.thickness,
                   knot.y - attribute.thickness,
                   width, width);
}

Point Editor::normalizeKnot (const Point& position) const noexcept
{
    const auto bounds = getAdjustedBounds();

    return {
        (position.x - bounds.getX()) / bounds.getWidth(),
        (position.y - bounds.getY()) / bounds.getHeight()};
}

juce::Rectangle< float > Editor::getAdjustedBounds() const
{
    return getLocalBounds().toFloat().reduced (attributes.getThickestThickness());
}

void Editor::resized()
{
    points.resize (to_size_f (static_cast< int > (getAdjustedBounds().getWidth())));
}

void Editor::timerCallback()
{
    knots.sort();
    updateCurve();
    repaint();
}

void Editor::mouseWheelMove (const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)
{
    const auto direc = wheel.deltaY > 0 ? 1.f : -1.f;
    select.addWidth (direc * scrollSensitivity);
    repaint();
}

void Editor::mouseMove (const juce::MouseEvent& evt)
{
    select.setX (evt.position.x);
    repaint();
}

void Editor::mouseDown (const juce::MouseEvent&)
{
    select();
    repaint();
}

void Editor::mouseDrag (const juce::MouseEvent& evt)
{
    select.setX (evt.position.x);
    select.move (normalizeKnot (evt.getOffsetFromDragStart().toFloat()));
}

void Editor::mouseUp (const juce::MouseEvent& evt)
{
    bool wu = false;

    if (evt.mouseWasDraggedSinceMouseDown())
    {
        wu = knots.removeOffLimits();
    }
    else
    {
        const auto pos = normalizeKnot (evt.position);
        if (evt.mods.isLeftButtonDown())
        {
            if (pos.x > 0.f && pos.x < 1.f)
            {
                knots.add (pos);
                wu = true;
            }
        }
        else
        {
            wu = knots.remove (select.getRange());
        }
    }

    select.deselect();
    if (! wu) repaint();
}

void Editor::mouseExit (const juce::MouseEvent&)
{
    select.mouseExited();
    repaint();
}

void Editor::updateCurve()
{
    knots.makeSpline (points);

    const auto bounds = getAdjustedBounds();
    const auto height = bounds.getHeight();
    const auto y      = bounds.getY();

    for (auto& point : points)
        point = point * height + y;
}

}  // namespace bav::spline
