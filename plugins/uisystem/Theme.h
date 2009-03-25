
class ThemePackage
{


protected:
    Theme::Array themes;
};

class Theme : public Object
{
public:
    virtual void drawWindow(Window* window) = 0;

    virtual bool canHandle(Window* window)
    {
        return false;
    }

protected:
    String name;
};

class ThemeRect : public Object
{
protected:
    s32 type;
    RectF rect;
};