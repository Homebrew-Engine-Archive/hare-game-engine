

void ButtonTheme::drawWindow(Window* window)
{
    if (!window)
        return;

    Button* button = (Button*)window;

    RectF* rect = NULL;

    if (!button->isEnabled())
    {
        rect = &disabledRect;
    }
    else if (button->isPushed())
    {
        rect = &pushedRect;
    }
    else if (button->isHovering())
    {
        rect = &hoverRect;
    }
    else
    {
        rect = &normalRect;
    }

    // TODO: draw material with rect
    //getCanvas()->drawImage(, material);
}