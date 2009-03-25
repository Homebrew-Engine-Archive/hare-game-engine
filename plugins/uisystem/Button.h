


class Button : public Window
{
public:
    bool isPushed() const
    {
        return pushed;
    }
    bool isHovering() const
    {
        return hovering;
    }

protected:
    bool pushed;
    bool hovering;
};

class ButtonTheme : public Theme
{
public:
     virtual void drawWindow(Window* window);

     virtual bool canHandle(Window* window)
     {
         return window && window->isA(&Button::CLASS_INFO);
     }

protected:
    Material::Ptr material;

    RectF normalRect;
    RectF hoverRect;
    RectF pushedRect;
    RectF disabledRect;
};